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

/*! \file pltf_gpio.c
 *
 *  \author
 *
 *  \brief Implementation of GPIO specific functions and its helper functions.
 *  
 */

/*
 ******************************************************************************
 * INCLUDES
 ******************************************************************************
 */
#include <stdio.h>
#include <poll.h>
#include <pthread.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#include <sys/stat.h>
#include <linux/gpio.h>
#include <sys/ioctl.h>

#include <semaphore.h>
#include "platform.h"

/* Extern semaphore to signal once an interrupt occurs */
extern sem_t rfal_sem;

/*
 ******************************************************************************
 * DEFINES
 ******************************************************************************
 */
/* Max size of file path to access */
#define SIZE    60


/*
 ******************************************************************************
 * STATIC VARIABLES
 ******************************************************************************
 */
static bool isGPIOInit = false;

int fd_GPIOs[NB_GPIO_LINES]; /* Array of file descriptors for each GPIO to control */

static pthread_mutex_t lock;
static pthread_mutex_t lockWorker;

/*
 ******************************************************************************
 * GLOBAL AND HELPER FUNCTIONS
 ******************************************************************************
 */

static int gpio_get_lineevent(int fd_gpiochip, int pin_no)
{
    struct gpioevent_request req;

    memset(&req, 0, sizeof(req));
    req.lineoffset  = pin_no;
    req.handleflags = GPIOHANDLE_REQUEST_INPUT;
    req.eventflags  = GPIOEVENT_REQUEST_RISING_EDGE;
    strcpy(req.consumer_label, "st25r391x_irq");;
    int ret = ioctl(fd_gpiochip, GPIO_GET_LINEEVENT_IOCTL, &req);
    if (ret < 0) {
        fprintf(stderr, "Failed to get line event for GPIO pin%d (ret %d)\n", pin_no, ret);
        return ret;
    }

    return req.fd;
}

static int gpio_get_linehandle(int fd_gpiochip, int direction, int pin_no)
{
    struct gpiohandle_request req;

    memset(&req, 0, sizeof(req));
    req.flags = (direction == GPIOHANDLE_REQUEST_INPUT ? GPIOHANDLE_REQUEST_INPUT : GPIOHANDLE_REQUEST_OUTPUT);
    req.lines = 1;
    req.lineoffsets[0] = pin_no;
    if (direction == GPIOHANDLE_REQUEST_INPUT) {
        strcpy(req.consumer_label, "st25r391x_in");
    }
    else {
        strcpy(req.consumer_label, "st25r391x_out");
    }
    int ret = ioctl(fd_gpiochip, GPIO_GET_LINEHANDLE_IOCTL, &req);
    if (ret < 0) {
        fprintf(stderr, "Failed to get line handle for GPIO pin%d (ret %d)\n", pin_no, ret);
        return ret;
    }

    return req.fd;
}

ReturnCode gpio_init(void)
{
    int ret = 0;
    char gpiochip_file[SIZE];

    if (isGPIOInit) {
        return ERR_NONE;
    }

    sprintf(gpiochip_file, "/dev/gpiochip%d", ST25R_INT_PORT); /* Open /dev/gpiochip0 */
    int fd = open(gpiochip_file, O_RDONLY);
    if (fd < 0) {
        ret = -errno;
        fprintf(stderr, "Failed to open %s\n", gpiochip_file);
        return ret;
    }

    /* Initialize file descriptor for each GPIO used by the platform */
    ST25R_INT_PORT          = gpio_get_lineevent(fd, ST25R_INT_PIN);
    PLATFORM_LED_FIELD_PORT = gpio_get_linehandle(fd, GPIOHANDLE_REQUEST_OUTPUT, PLATFORM_LED_FIELD_PIN);
    PLATFORM_LED_A_PORT     = gpio_get_linehandle(fd, GPIOHANDLE_REQUEST_OUTPUT, PLATFORM_LED_A_PIN);
    PLATFORM_LED_B_PORT     = gpio_get_linehandle(fd, GPIOHANDLE_REQUEST_OUTPUT, PLATFORM_LED_B_PIN);
    PLATFORM_LED_F_PORT     = gpio_get_linehandle(fd, GPIOHANDLE_REQUEST_OUTPUT, PLATFORM_LED_F_PIN);
    PLATFORM_LED_V_PORT     = gpio_get_linehandle(fd, GPIOHANDLE_REQUEST_OUTPUT, PLATFORM_LED_V_PIN);
    PLATFORM_LED_AP2P_PORT  = gpio_get_linehandle(fd, GPIOHANDLE_REQUEST_OUTPUT, PLATFORM_LED_AP2P_PIN);

    /* Close /dev/gpiochipx */
    close(fd);

    /* Check each file descriptor is initialized properly */
    for (int i = 0; i < NB_GPIO_LINES; i++)
    {
        if (fd_GPIOs[i] < 0) {
            fprintf(stderr, "GPIO init failed\n");
            return ERR_INVALID_HANDLE;
        }
    }

    ret = pthread_mutex_init(&lock, NULL);
    if (ret != 0) {
        fprintf(stderr, "Failed to initialize mutex protecting interrupt status\n");
        return ret;
    }
    
    ret = pthread_mutex_init(&lockWorker, NULL);
    if (ret != 0) {
        fprintf(stderr, "Failed to initialize mutex protecting worker\n");
        return ret;
    }

    isGPIOInit = true;
    return ERR_NONE;
}


GPIO_PinState gpio_readpin(int port, int pin_no)
{
    struct gpiohandle_data data;

    /* First check whether GPIOInit is done */
    if (!isGPIOInit) {
        ReturnCode ret = gpio_init();
        if (ret != ERR_NONE) {
            return GPIO_PIN_RESET;
        }
    }

    /* The port is used to contain the file descriptor of a given pin! */
    /* So use the port for ioctl */
    int ret = ioctl(port, GPIOHANDLE_GET_LINE_VALUES_IOCTL, &data);
    if (ret < 0) {
        fprintf(stderr, "Error: while reading GPIO pin state\n");
        return ERR_IO;
    }

    GPIO_PinState state = (data.values[0] == GPIO_PIN_RESET ? GPIO_PIN_RESET : GPIO_PIN_SET);

    return state;
}

void* pthread_func()
{
    struct gpioevent_data event;

    /* First check whether GPIOInit is done */
    if (!isGPIOInit) {
        fprintf(stderr, "GPIO is not initialized\n");
        return NULL;
    }

    /* poll interrupt line forever */
    while (true)
    {
        /* read() blocks the calling thread until some data becomes available */
        /* The port is used to contain the file descriptor of a given pin! */
        ssize_t s = read(ST25R_INT_PORT, &event, sizeof(event));

        /* Call RFAL Isr */
        platformIsr();

        /* Unlock Semaphore */
        sem_post(&rfal_sem);
    }

    return NULL;
}

ReturnCode interrupt_init(void)
{
    pthread_t intr_thread;
    struct sched_param params;
    int ret;

    /* Create a pthread polling for interrupt */
    ret = pthread_create(&intr_thread, NULL, pthread_func, NULL);
    if (ret) {
        fprintf(stderr, "Error: poll thread creation %d\n", ret);
        return ERR_IO;
    }

    /* Assign highest priority to the polling thread */
    params.sched_priority = sched_get_priority_max(SCHED_FIFO);
    ret = pthread_setschedparam(intr_thread, SCHED_FIFO, &params);
    if (ret) {
        fprintf(stderr, "Error: assigning high priority to polling thread\n");
        return ERR_IO;
    }

    return ERR_NONE;
}

void gpio_set_value(int port, int pin_no, uint8_t value)
{
    struct gpiohandle_data data;

    /* First check whether GPIOInit is done */
    if (!isGPIOInit) {
        ReturnCode ret = gpio_init();
        if (ret != ERR_NONE) {
            return;
        }
    }

    memset(&data, 0, sizeof(data));
    data.values[0] = (value == GPIO_PIN_RESET ? GPIO_PIN_RESET : GPIO_PIN_SET);

    /* The port is used to contain the file descriptor of a given pin! */
    int ret = ioctl(port, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &data);
    if (ret < 0) {
        fprintf(stderr, "Failed to set pin%d value %d (ret %d)\n", pin_no, data.values[0], ret);
    }
}


void gpio_set(int port, int pin_no)
{
    gpio_set_value(port, pin_no, GPIO_PIN_SET);
}

void gpio_clear(int port, int pin_no)
{
    gpio_set_value(port, pin_no, GPIO_PIN_RESET);
}

void pltf_protect_interrupt_status(void)
{
    pthread_mutex_lock(&lock);
}

void pltf_unprotect_interrupt_status(void)
{
    pthread_mutex_unlock(&lock);
}

void pltf_protect_worker(void)
{
    pthread_mutex_lock(&lockWorker);
}

void pltf_unprotect_worker(void)
{
    pthread_mutex_unlock(&lockWorker);
}