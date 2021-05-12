/******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2019 STMicroelectronics</center></h2>
  *
  * Licensed under ST MYLIBERTY SOFTWARE LICENSE AGREEMENT (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/myliberty
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied,
  * AND SPECIFICALLY DISCLAIMING THE IMPLIED WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE, AND NON-INFRINGEMENT.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
******************************************************************************/

/*! \file pltf_timer.c
 *
 *  \brief SW Timer implementation
 *
 *  \author Gustavo Patricio
 *
 *   This module makes use of a System Tick in millisconds and provides
 *   an abstraction for SW timers.
 *   Modified to implement timers on Linux platform.
 *
 */

/*
******************************************************************************
* INCLUDES
******************************************************************************
*/
#include <time.h>
#include <signal.h>
#include <pthread.h>
#include <semaphore.h>
#include "pltf_timer.h"
#include "platform.h"

/*
******************************************************************************
* LOCAL DEFINES
******************************************************************************
*/

/* Number of timers supported */
#define PLT_TIMER_MAX 10U


/*
******************************************************************************
* GLOBAL AND HELPER FUNCTIONS
******************************************************************************
*/

/* Extern semaphore to signal once a timer expires */
extern sem_t rfal_sem;

/* Define an array of POSIX timers */
timer_t platformTimers[PLT_TIMER_MAX];


/*****************************************************************************/
static uint32_t ts2millisec(struct timespec* ts)
{
    return ((ts->tv_sec * (uint32_t)1000) + (ts->tv_nsec/1000000));
}


/****************************************************************************/
uint32_t platformGetSysTick_linux()
{
    struct timespec cur_ts;
    clock_gettime(CLOCK_MONOTONIC, &cur_ts);
    return ts2millisec(&cur_ts);
}


/****************************************************************************/
static void timer_handler(union sigval sv)
{
    /* Unlock Semaphore */
    sem_post(&rfal_sem);
}


/****************************************************************************/
uint32_t plfTimerCreate(uint32_t time)
{
    uint32_t timer = UINT32_MAX;
    for (uint32_t i = 0; i < PLT_TIMER_MAX; i++)
    {
        /* Look for the first timer not used (handle is 0) */
        if (platformTimers[i] == 0)
        {
            timer = i;
            break;
        }
    }
    if (timer >= PLT_TIMER_MAX)
    {
        platformLog("platformTimerCreate: timer %d creation failed\r\n", timer);
        exit(EXIT_FAILURE);
        return 0;
    }

    struct sigevent sevp = {
        .sigev_notify          = SIGEV_THREAD,
        .sigev_notify_function = timer_handler,
    };
    int rc = timer_create(CLOCK_MONOTONIC, &sevp, &platformTimers[timer]);
    if (rc != 0)
    {
        platformLog("platformTimerCreate: timer_create() failed (rc = %d)\r\n", rc);
        exit(EXIT_FAILURE);
        return 0;
    }

    /* Go beyond nearest tick */
    time += 1;
    struct itimerspec its = {
       .it_interval.tv_sec  = 0,               /* Timer interval = 0, i.e. not a One-shot timer */
       .it_interval.tv_nsec = 0,

       .it_value.tv_sec     =  time / 1000U,   /* Initial expiration */
       .it_value.tv_nsec    = (time % 1000U) * 1000000U,
    };

    rc = timer_settime(platformTimers[timer], CLOCK_REALTIME, &its, NULL);
    if (rc != 0)
    {
        platformLog("platformTimerCreate: timer_settime() timer %d failed (rc = %d) (param %d=>%d/%d)\r\n", timer, rc, time, its.it_value.tv_sec, its.it_value.tv_nsec);
        exit(EXIT_FAILURE);
        return 0;
    }

    /* Start from index 1, don't use timer 0 as it has a specifing meaning in RFAL */
    return timer + 1U;
}


/****************************************************************************/
bool plfTimerIsExpired(uint32_t timer)
{
    /* Timer 0 as a specific meaning in RFAL, don't use it */
    if (timer == 0U)
    {
        return true;
    }
    timer--;
    if (timer >= PLT_TIMER_MAX)
    {
        platformLog("platformTimerIsExpired: invalid timer %d\r\n", timer);
        exit(EXIT_FAILURE);
        return true;
    }

    struct itimerspec its;
    timer_gettime(platformTimers[timer], &its);
    return ( (its.it_value.tv_sec == 0) && (its.it_value.tv_nsec == 0) );
}


/****************************************************************************/
void plfTimerDestroy(uint32_t timer)
{
    /* Timer 0 as a specific meaning in RFAL, don't use it */
    if (timer == 0U)
    {
        return;
    }
    timer--;
    if (timer >= PLT_TIMER_MAX)
    {
        platformLog("platformTimerDestroy: invalid timer %d\r\n", timer);
        exit(EXIT_FAILURE);
        return;
    }

    if (timer_delete(platformTimers[timer]) != 0)
    {
        platformLog("timer_delete() %d failed\r\n", timer);
        exit(EXIT_FAILURE);
    }

    /* Mark it back as an available timer */
    platformTimers[timer] = 0;
}
