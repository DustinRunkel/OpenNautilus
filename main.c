/*
 * FreeRTOS Kernel <DEVELOPMENT BRANCH>
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

/*
 * This is a simple main that will start the FreeRTOS-Kernel and run a periodic task
 * that only delays if compiled with the template port, this project will do nothing.
 * For more information on getting started please look here:
 * https://www.freertos.org/Documentation/01-FreeRTOS-quick-start/01-Beginners-guide/02-Quick-start-guide
 */

/* FreeRTOS includes. */
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <timers.h>
#include <semphr.h>

/* Standard includes. */
#include <stdio.h>

/*pi includes*/
#include "pico/stdio.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"

#define GPIO_PIN_UP 0
#define GPIO_PIN_BALLAST_PUMP 1

/*-----------------------------------------------------------*/

static void exampleTask( void * parameters ) __attribute__( ( noreturn ) );
static void controlTask( void * parameters ) __attribute__( ( noreturn ) );
/*-----------------------------------------------------------*/

static void exampleTask( void * parameters )
{
    /* Unused parameters. */
    ( void ) parameters;

    for( ; ; )
    {
        /* Example Task Code */
        printf("[Example Task]: Example log\n");
        vTaskDelay( 100 ); /* delay 100 ticks */
    }
}

static void controlTask( void * parameters )
{
    int status
    (void) parameters;
    for( ; ; )
    {
        status = gpio_get(GPIO_PIN_UP);
        if(status)
        {
            gpio_put(GPIO_PIN_BALLAST_PUMP, true);
        } else 
        {
            gpio_put(GPIO_PIN_BALLAST_PUMP, false);
        }
        /* Example Task Code */
        printf("[Control Task]: Control Status pump %d\n", status);
        vTaskDelay( 1 );
    }
}
/*-----------------------------------------------------------*/

int main( void )
{

    stdio_init_all();
    gpio_init(GPIO_PIN_UP);
    gpio_init(GPIO_PIN_BALLAST_PUMP);
    gpio_set_dir(GPIO_PIN_UP, GPIO_IN);
    gpio_set_dir(GPIO_PIN_BALLAST_PUMP, GPIO_OUT);
    

    while (!stdio_usb_connected()) {
        sleep_ms(10); // Wait for USB serial terminal to connect
    }
    static StaticTask_t exampleTaskTCB;
    static StaticTask_t controlTaskTCB;
    static StackType_t exampleTaskStack[ configMINIMAL_STACK_SIZE ];
    static StackType_t controlTaskStack[ configMINIMAL_STACK_SIZE ];

    ( void ) printf( "Example FreeRTOS Project\n" );

    ( void ) xTaskCreateStatic( &exampleTask,
                                "example",
                                configMINIMAL_STACK_SIZE,
                                NULL,
                                configMAX_PRIORITIES - 1U,
                                &( exampleTaskStack[ 0 ] ),
                                &( exampleTaskTCB ) );
    
    ( void ) xTaskCreateStatic( &controlTask,
                            "control",
                            configMINIMAL_STACK_SIZE,
                            NULL,
                            configMAX_PRIORITIES - 1U,
                            &( controlTaskStack[ 0 ] ),
                            &( controlTaskTCB ) );

    /* Start the scheduler. */
    vTaskStartScheduler();

    for( ; ; )
    {
        /* Should not reach here. */
    }

    return 0;
}
/*-----------------------------------------------------------*/

#if ( configCHECK_FOR_STACK_OVERFLOW > 0 )

    void vApplicationStackOverflowHook( TaskHandle_t xTask,
                                        char * pcTaskName )
    {
        /* Check pcTaskName for the name of the offending task,
         * or pxCurrentTCB if pcTaskName has itself been corrupted. */
        ( void ) xTask;
        ( void ) pcTaskName;
    }

#endif /* #if ( configCHECK_FOR_STACK_OVERFLOW > 0 ) */
/*-----------------------------------------------------------*/
