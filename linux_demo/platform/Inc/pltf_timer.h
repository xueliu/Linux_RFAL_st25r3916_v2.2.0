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

/*! \file pltf_timer.h
 *
 *  \brief SW Timer implementation header file
 *   
 *   This module makes use of a System Tick in millisconds and provides
 *   an abstraction for SW timers
 *
 */
 
#ifndef PLATFORM_TIMER_H
#define PLATFORM_TIMER_H
 
 /*
******************************************************************************
* INCLUDES
******************************************************************************
*/
#include <stdint.h>
#include <stdbool.h>

/*
******************************************************************************
* GLOBAL MACROS
******************************************************************************
*/


/*
******************************************************************************
* GLOBAL DEFINES
******************************************************************************
*/
uint32_t platformGetSysTick_linux();
 
 /*!
 *****************************************************************************
 * \brief  Creates a Timer
 *
 * This method creates a timer that will expire after the given amount
 * of milliseconds.
 *
 * \see timersIsExpired
 *
 * \param[in]  time : time/duration in milliseconds for the timer
 *
 * \return u32 : The new timer's handle
 *****************************************************************************
 */
uint32_t plfTimerCreate(uint32_t time);

/*!
 *****************************************************************************
 * \brief  Checks if a Timer is expired
 *  
 * This method checks if a timer has already expired.
 * Based on the given timer previously calculated it checks if this timer
 * has already elapsed
 * 
 * \see plfTimerCreate
 *
 * \param[in]  timer : the timer to check
 *
 * \return true  : timer has already expired
 * \return false : timer is still running
 *****************************************************************************
 */
bool plfTimerIsExpired(uint32_t timer);

/*!
 *****************************************************************************
 * \brief  Destroys the given timer
 *  
 * This method destroys the timer previously created.
 * 
 * \see plfTimerCreate
 *
 * \param[in]  timer : the timer to destroy
 *
 *****************************************************************************
 */
void plfTimerDestroy(uint32_t timer);

#endif /* PLATFORM_TIMER_H */
