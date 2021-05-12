/******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2020 STMicroelectronics</center></h2>
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
/*! \file
 *
 *  \author 
 *
 *  \brief Platform header file. Defining platform independent functionality.
 *
 */


/*
 *      PROJECT:   
 *      $Revision: $
 *      LANGUAGE:  ISO C99
 */

/*! \file platform.h
 *
 *  \author Gustavo Patricio
 *
 *  \brief Platform specific definition layer  
 *  
 *  This should contain all platform and hardware specifics such as 
 *  GPIO assignment, system resources, locks, IRQs, etc
 *  
 *  Each distinct platform/system/board must provide this definitions 
 *  for all SW layers to use
 *
 *  Modified for Linux platform.
 *  
 */

#ifndef PLATFORM_H
#define PLATFORM_H

/*
******************************************************************************
* INCLUDES
******************************************************************************
*/
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include "pltf_timer.h"
#include "pltf_spi.h"
#include "pltf_gpio.h"
#include "st25r3916_irq.h"
#include "logger.h"

/*
******************************************************************************
* GLOBAL DEFINES
******************************************************************************
*/

/* X-NUCLEO-NFC06 GPIO/LEDs configuration
    IRQ_MCU      A0 = CN8.1
    MCU_LED1 PA1 A1 = CN8.2
    MCU_LED2 PA4 A2 = CN8.3
    MCU_LED3 PB0 A3 = CN8.4
    MCU_LED4 PC1 A4 = CN8.5
    MCU_LED5 PC0 A5 = CN8.6
    MCU_LED6 D7  D7 = CN9.8

    Jumpers on the Waveshare ARPI600 remaps CN8 connector as follow:
    A0 = CE1
    A1 = P21
    A2 = P22
    A3 = P23
    A4 = P24
    A5 = P25

    D7 = P5

    According to the ARPI600 MCU/Pi connector mapping:
    CE1 = 7  (IRQ_MCU)
    P21 = 5  (MCU_LED1 = LED F)
    P22 = 6  (MCU_LED2 = LED B)
    P23 = 13 (MCU_LED3 = LED A)
    P24 = 19 (MCU_LED4 = LED V)
    P25 = 26 (MCU_LED5 = LED AP2P)
    P5  = 24 (MCU_LED6 = TX)

*/
#define PLATFORM_LED_A_PIN               13
#define PLATFORM_LED_B_PIN                6
#define PLATFORM_LED_F_PIN                5
#define PLATFORM_LED_V_PIN               19
#define PLATFORM_LED_AP2P_PIN            26

/* Number of GPIO lines to control */
#define NB_GPIO_LINES                    7U
extern int fd_GPIOs[NB_GPIO_LINES]; /* Array of file descriptors for each GPIO to control */

#define PLATFORM_LED_A_PORT              (fd_GPIOs[2])
#define PLATFORM_LED_B_PORT              (fd_GPIOs[3])
#define PLATFORM_LED_F_PORT              (fd_GPIOs[4])
#define PLATFORM_LED_V_PORT              (fd_GPIOs[5])
#define PLATFORM_LED_AP2P_PORT           (fd_GPIOs[6])

#define LED_FIELD_Pin                    24
#define LED_FIELD_GPIO_Port              (fd_GPIOs[1])

#define ST25R_SS_PIN                                      /*!< GPIO pin used for ST25R SPI SS                    */
#define ST25R_SS_PORT                                     /*!< GPIO port used for ST25R SPI SS port              */

/* Set the GPIO pin number used as interrupt line to receive interrupts from ST25R */
#define ST25R_INT_PIN               7                     /*!< GPIO pin used for ST25R External Interrupt        */
#define ST25R_INT_PORT              (fd_GPIOs[0])         /*!< GPIO port used for ST25R External Interrupt       */

#ifdef LED_FIELD_Pin
#define PLATFORM_LED_FIELD_PIN      LED_FIELD_Pin         /*!< GPIO pin used as field LED                        */
#endif

#ifdef LED_FIELD_GPIO_Port
#define PLATFORM_LED_FIELD_PORT     LED_FIELD_GPIO_Port   /*!< GPIO port used as field LED                       */
#endif


/*
******************************************************************************
* GLOBAL MACROS
******************************************************************************
*/

#define ST25R_COM_SINGLETXRX                              /*!< Enable single SPI frame transmission */

#define platformProtectST25RComm()                pltf_protect_com()                                      /*!< Protect unique access to ST25R communication channel - IRQ disable on single thread environment (MCU) ; Mutex lock on a multi thread environment */
#define platformUnprotectST25RComm()              pltf_unprotect_com()                                    /*!< Unprotect unique access to ST25R communication channel - IRQ enable on a single thread environment (MCU) ; Mutex unlock on a multi thread environment         */

#define platformProtectWorker()                   pltf_protect_worker()                                   /*!< Protect RFAL Worker/Task/Process from concurrent execution on multi thread platforms   */
#define platformUnprotectWorker()                 pltf_unprotect_worker()                                 /*!< Unprotect RFAL Worker/Task/Process from concurrent execution on multi thread platforms */

#define platformProtectST25RIrqStatus()           pltf_protect_interrupt_status()                         /*!< Acquire the lock for safe access of RFAL interrupt status variable */
#define platformUnprotectST25RIrqStatus()         pltf_unprotect_interrupt_status()                       /*!< Release the lock aquired for safe accessing of RFAL interrupt status variable */

#define platformIrqST25R3916SetCallback(cb)
#define platformIrqST25R3916PinInitialize()


#define platformSpiSelect()                                             /*!< SPI SS\CS: Chip|Slave Select */
#define platformSpiDeselect()                                           /*!< SPI SS\CS: Chip|Slave Deselect */

#define platformIsr()                                 st25r3916Isr()

#define platformGpioSet(port, pin)                    gpio_set(port, pin)       /*!< Turns the given GPIO High */
#define platformGpioClear(port, pin)                  gpio_clear(port, pin)     /*!< Turns the given GPIO Low  */
#define platformGpioToggle(port, pin)                 (platformGpioIsHigh(port, pin) ? platformGpioClear( port, pin) : platformGpioSet(port, pin))  /*!< Toogles the given GPIO    */
#define platformGpioIsHigh(port, pin)                 (gpio_readpin(port, pin) == GPIO_PIN_SET)               /*!< Checks if the given GPIO is High */
#define platformGpioIsLow(port, pin)                  (!platformGpioIsHigh(port, pin))                        /*!< Checks if the given GPIO is Low  */

#define platformLedsInitialize()                                                                               /*!< Initializes the pins used as LEDs to outputs*/
#define platformLedOff(port, pin)                     platformGpioClear(port, pin)                             /*!< Turns the given LED Off */ /* Support for old board version MB1325-A */
#define platformLedOn(port, pin)                      platformGpioSet(port, pin)                               /*!< Turns the given LED On  */ /* Support for old board version MB1325-A */

#define platformTimerCreate(t)                        plfTimerCreate(t)                                        /*!< Create a timer with the given time (ms)     */
#define platformTimerIsExpired(t)                     plfTimerIsExpired(t)                                     /*!< Checks if the given timer is expired        */
#define platformTimerDestroy(t)                       plfTimerDestroy(t)                                       /*!< Destroy a timer                             */

#define platformDelay(t)                              usleep((t)*1000U)                                        /*!< Performs a delay for the given time (ms)    */

#define platformGetSysTick()                          platformGetSysTick_linux()                               /*!< Get System Tick (1 tick = 1 ms)             */

#define platformSpiTxRx(txBuf, rxBuf, len)            spiTxRx(txBuf, rxBuf, len)                               /*!< SPI transceive                              */


#define platformI2CTx(txBuf, len)                                                                              /*!< I2C Transmit                                */
#define platformI2CRx(txBuf, len)                                                                              /*!< I2C Receive                                 */
#define platformI2CStart()                                                                                     /*!< I2C Start condition                         */
#define platformI2CStop()                                                                                      /*!< I2C Stop condition                          */
#define platformI2CRepeatStart()                                                                               /*!< I2C Repeat Start                            */
#define platformI2CSlaveAddrWR(add)                                                                            /*!< I2C Slave address for Write operation       */
#define platformI2CSlaveAddrRD(add)                                                                            /*!< I2C Slave address for Read operation        */

#define platformLog(...)                              do { printf(__VA_ARGS__); fflush(stdout); } while(0);    /*!< Log  method                                 */

/*
******************************************************************************
* RFAL FEATURES CONFIGURATION
******************************************************************************
*/

#define RFAL_FEATURE_LISTEN_MODE               true       /*!< Enable/Disable RFAL support for Listen Mode                               */
#define RFAL_FEATURE_WAKEUP_MODE               true       /*!< Enable/Disable RFAL support for the Wake-Up mode                          */
#define RFAL_FEATURE_LOWPOWER_MODE             false      /*!< Enable/Disable RFAL support for the Low Power mode                        */
#define RFAL_FEATURE_NFCA                      true       /*!< Enable/Disable RFAL support for NFC-A (ISO14443A)                         */
#define RFAL_FEATURE_NFCB                      true       /*!< Enable/Disable RFAL support for NFC-B (ISO14443B)                         */
#define RFAL_FEATURE_NFCF                      true       /*!< Enable/Disable RFAL support for NFC-F (FeliCa)                            */
#define RFAL_FEATURE_NFCV                      true       /*!< Enable/Disable RFAL support for NFC-V (ISO15693)                          */
#define RFAL_FEATURE_T1T                       true       /*!< Enable/Disable RFAL support for T1T (Topaz)                               */
#define RFAL_FEATURE_T2T                       true       /*!< Enable/Disable RFAL support for T2T (MIFARE Ultralight)                   */
#define RFAL_FEATURE_T4T                       true       /*!< Enable/Disable RFAL support for T4T                                       */
#define RFAL_FEATURE_ST25TB                    true       /*!< Enable/Disable RFAL support for ST25TB                                    */
#define RFAL_FEATURE_ST25xV                    true       /*!< Enable/Disable RFAL support for ST25TV/ST25DV                             */
#define RFAL_FEATURE_DYNAMIC_ANALOG_CONFIG     false      /*!< Enable/Disable Analog Configs to be dynamically updated (RAM)             */
#define RFAL_FEATURE_DPO                       false      /*!< Enable/Disable RFAL Dynamic Power Output support                          */
#define RFAL_FEATURE_ISO_DEP                   true       /*!< Enable/Disable RFAL support for ISO-DEP (ISO14443-4)                      */
#define RFAL_FEATURE_ISO_DEP_POLL              true       /*!< Enable/Disable RFAL support for Poller mode (PCD) ISO-DEP (ISO14443-4)    */
#define RFAL_FEATURE_ISO_DEP_LISTEN            true       /*!< Enable/Disable RFAL support for Listen mode (PICC) ISO-DEP (ISO14443-4)   */
#define RFAL_FEATURE_NFC_DEP                   true       /*!< Enable/Disable RFAL support for NFC-DEP (NFCIP1/P2P)                      */


#define RFAL_FEATURE_ISO_DEP_IBLOCK_MAX_LEN    256U       /*!< ISO-DEP I-Block max length. Please use values as defined by rfalIsoDepFSx */
#define RFAL_FEATURE_NFC_DEP_BLOCK_MAX_LEN     254U       /*!< NFC-DEP Block/Payload length. Allowed values: 64, 128, 192, 254           */
#define RFAL_FEATURE_NFC_RF_BUF_LEN            256U       /*!< RF buffer length used by RFAL NFC layer                                   */

#define RFAL_FEATURE_ISO_DEP_APDU_MAX_LEN      512U       /*!< ISO-DEP APDU max length.                                                  */
#define RFAL_FEATURE_NFC_DEP_PDU_MAX_LEN       512U       /*!< NFC-DEP PDU max length.                                                   */




/*
 ******************************************************************************
 * RFAL OPTIONAL MACROS            (Do not change)
 ******************************************************************************
 */

#ifndef platformProtectST25RIrqStatus
    #define platformProtectST25RIrqStatus()            /*!< Protect unique access to IRQ status var - IRQ disable on single thread environment (MCU) ; Mutex lock on a multi thread environment */
#endif /* platformProtectST25RIrqStatus */

#ifndef platformUnprotectST25RIrqStatus
    #define platformUnprotectST25RIrqStatus()          /*!< Unprotect the IRQ status var - IRQ enable on a single thread environment (MCU) ; Mutex unlock on a multi thread environment         */
#endif /* platformUnprotectST25RIrqStatus */

#ifndef platformProtectWorker
    #define platformProtectWorker()                    /* Protect RFAL Worker/Task/Process from concurrent execution on multi thread platforms   */
#endif /* platformProtectWorker */

#ifndef platformUnprotectWorker
    #define platformUnprotectWorker()                  /* Unprotect RFAL Worker/Task/Process from concurrent execution on multi thread platforms */
#endif /* platformUnprotectWorker */

#ifndef platformIrqST25RPinInitialize
    #define platformIrqST25RPinInitialize()            /*!< Initializes ST25R IRQ pin                    */
#endif /* platformIrqST25RPinInitialize */

#ifndef platformIrqST25RSetCallback
    #define platformIrqST25RSetCallback( cb )          /*!< Sets ST25R ISR callback                      */
#endif /* platformIrqST25RSetCallback */

#ifndef platformLedsInitialize
    #define platformLedsInitialize()                   /*!< Initializes the pins used as LEDs to outputs */
#endif /* platformLedsInitialize */

#ifndef platformLedOff
    #define platformLedOff( port, pin )                /*!< Turns the given LED Off                      */
#endif /* platformLedOff */                                                                              
                                                                                                         
#ifndef platformLedOn                                                                                    
    #define platformLedOn( port, pin )                 /*!< Turns the given LED On                       */
#endif /* platformLedOn */                                                                               
                                                                                                         
#ifndef platformLedToogle                                                                                
    #define platformLedToogle( port, pin )             /*!< Toggles the given LED                        */
#endif /* platformLedToogle */                                                                           
                                                                                                         
#ifndef platformGetSysTick                                                                               
    #define platformGetSysTick()                       /*!< Get System Tick (1 tick = 1 ms)              */
#endif /* platformGetSysTick */                                                                          
                                                                                                         
#ifndef platformTimerDestroy                                                                             
    #define platformTimerDestroy( timer )              /*!< Stops and released the given timer           */
#endif /* platformTimerDestroy */

#ifndef platformAssert                                                                             
    #define platformAssert( exp )                      /*!< Asserts whether the given expression is true */
#endif /* platformAssert */

#ifndef platformErrorHandle                                                                             
    #define platformErrorHandle()                      /*!< Global error handler or trap                 */
#endif /* platformErrorHandle */


#endif /* PLATFORM_H */

