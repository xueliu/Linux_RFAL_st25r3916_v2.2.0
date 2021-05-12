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

/*
 ******************************************************************************
 * INCLUDES
 ******************************************************************************
 */
#include "platform.h"
#include "demo.h"
#include <pthread.h>
#include <sys/time.h>
#include <semaphore.h>
#include <errno.h>

/*
 ******************************************************************************
 * DEFINES
 ******************************************************************************
 */

/* number of nanoseconds in a millisecond */
#define NS_TO_MS (1000000U)

/*
 ******************************************************************************
 * MAIN FUNCTION
 ******************************************************************************
 */

/* Semaphore used to suspend main thread until:
   - either an interrupt occurs (on the IRQ_MCU GPIO)
   - or a timer expires */
sem_t rfal_sem;

bool add_time(struct timespec* ts, struct timespec* delay, struct timespec* res)
{
    bool overflow = false;

    res->tv_sec   = ts->tv_sec;
    res->tv_nsec  = ts->tv_nsec;
    res->tv_sec  += delay->tv_sec;
    res->tv_nsec += delay->tv_nsec;

    if (res->tv_nsec >= 1000000000)  /* nanoseconds must be in range [0 .. 999999999] */
    {
        res->tv_sec  += 1U;
        res->tv_nsec -= 1000000000;
        overflow = true;
    }
    return overflow;
}

int main(void)
{
	platformLog("Welcome to the ST25R3916 NFC Demo on Linux.\r\n");
	platformLog("Scanning for NFC technologies...\r\n");

	int ret = 0;
	struct timespec sem_time;
	struct timespec ts_delay;
	struct timespec new_time;

	setlinebuf(stdout);

	/* Initialize the platform */
	/* Initialize GPIO */
	ret = gpio_init();
	if (ret != ERR_NONE)
		goto error;
	/* Initialize SPI */
	ret = spi_init();
	if (ret != ERR_NONE)
		goto error;
	/* Initialize interrupt mechanism */
	ret = interrupt_init();
	if (ret != ERR_NONE)
		goto error;

	/* Initialize the semaphore */
	ret = sem_init(&rfal_sem, 0, 0);
	if (ret != 0) {
		goto error;
	}

	/* Initialize RFAL and run the demo */
	/*
	* in case the rfal initalization failed signal it by flashing all LED
	* and stopping all operations
	*/
	int i = 3;
	bool ok = demoIni();
	while ((!ok) || (ok && i--))
	{
		platformLedOn(PLATFORM_LED_FIELD_PORT, PLATFORM_LED_FIELD_PIN);
		platformLedOn(PLATFORM_LED_A_PORT, PLATFORM_LED_A_PIN);
		platformLedOn(PLATFORM_LED_B_PORT, PLATFORM_LED_B_PIN);
		platformLedOn(PLATFORM_LED_F_PORT, PLATFORM_LED_F_PIN);
		platformLedOn(PLATFORM_LED_V_PORT, PLATFORM_LED_V_PIN);
		platformLedOn(PLATFORM_LED_AP2P_PORT, PLATFORM_LED_AP2P_PIN);
		platformDelay(ok ? 200U : 100U);
		platformLedOff(PLATFORM_LED_FIELD_PORT, PLATFORM_LED_FIELD_PIN);
		platformLedOff(PLATFORM_LED_A_PORT, PLATFORM_LED_A_PIN);
		platformLedOff(PLATFORM_LED_B_PORT, PLATFORM_LED_B_PIN);
		platformLedOff(PLATFORM_LED_F_PORT, PLATFORM_LED_F_PIN);
		platformLedOff(PLATFORM_LED_V_PORT, PLATFORM_LED_V_PIN);
		platformLedOff(PLATFORM_LED_AP2P_PORT, PLATFORM_LED_AP2P_PIN);
		platformDelay(ok ? 200U : 100U);
	}

	ts_delay.tv_sec  = 0;
	ts_delay.tv_nsec = 500U * NS_TO_MS;

	while(1)
	{
		demoCycle();
		/* Call the demo cycle twice to move forward the RFAL state machine */
		/* This avoids the timed semaphore to run into its timeout */
		demoCycle();

		/* Lock Semaphore to suspend this thread */
		clock_gettime(CLOCK_REALTIME, &sem_time);

		add_time(&sem_time, &ts_delay, &new_time);

		int err = sem_timedwait(&rfal_sem, &new_time);
		if ( (err != 0) && (errno != ETIMEDOUT) ) {
			platformLog("sem_timedwait error %d errno %d\r\n", err, errno);
		}
	}

	sem_destroy(&rfal_sem);

error:
	return ret;
}
