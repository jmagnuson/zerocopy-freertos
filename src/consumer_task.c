/****************************************************************************
*
* Copyright (C) 2014
* Written by Jon Magnuson <my.name at google's mail service>
* All Rights Reserved.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along
* with this program; if not, write to the Free Software Foundation, Inc.,
* 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*
****************************************************************************/

/* Standard includes. */
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "portable.h"

/* Application includes. */
#include "consumer_task.h"
#include "ringbuf.h"
#include "barcnt.h"
#include "queuemsg.h"


#define configCONSUMER_STACK_SIZE        configMINIMAL_STACK_SIZE // Stack size in words

#define tskCONSUMER_PRIORITY        (tskIDLE_PRIORITY + 1) // Offset from IDLE priority

//*****************************************************************************
//
// Main task
//
//*****************************************************************************
static void
prvConsumerTask(void *pvParameters)
{
	/* Variable declarations */
	QueueHandle_t queue = NULL;
	//Barrier* barrier = NULL;
	atomic_ringbuf_t* circular_buffer = NULL;
	unsigned int consumer_id = 0;
	queuemsg_t message = {NULL, NULL, NULL};
	portBASE_TYPE local_buffer[256];
	portBASE_TYPE local_buffer_length = 0;
	
	/* Variable initialization */
	queue =           ((xConsumerTaskParams*)pvParameters)->queue;
	circular_buffer = ((xConsumerTaskParams*)pvParameters)->circular_buffer;
	consumer_id =     ((xConsumerTaskParams*)pvParameters)->consumer_id;
	
	int barrier_result = 0;

	int pseudorandom_wait = consumer_id;

	for (;;)
	{
		__asm( "nop" );

		xQueueReceive( queue, &message, portMAX_DELAY );
	

		local_buffer_length = message.buffer_len;
		memcpy(local_buffer, message.buffer_ptr, local_buffer_length);

		{
			/* delay 0 - 490ms (simulate other tasks being done) */
			vTaskDelay(pseudorandom_wait * 10 / portTICK_RATE_MS);
			pseudorandom_wait = (2*pseudorandom_wait) % 50;
		}

		printf("c%02d: %s\n", consumer_id, (char*)local_buffer);

		barrier_result = dec_barcnt(message.consumer_count_barrier);

		if (barrier_result == 0)
		{
			// Last barrier, free up cbuff memory
			read_atomic_ringbuf( circular_buffer, NULL, local_buffer_length );
			// TODO: put into dec_barrier?
		}
		else if (barrier_result < 0)
		{
			// Shouldn't happen since consumer_count_barrier is protected by lock.
		}
	}

}

//*****************************************************************************
//
// Init task
//
//*****************************************************************************
extern unsigned long
init_consumer_task(void *pvParameters)
{
	xConsumerTaskParams *taskParams =
			( xConsumerTaskParams * ) pvPortMalloc( sizeof( xConsumerTaskParams ) );

	taskParams->queue =
			((xConsumerTaskInitParams*) pvParameters)->queue;
	taskParams->circular_buffer =
			((xConsumerTaskInitParams*) pvParameters)->circular_buffer;
	taskParams->consumer_id =
			((xConsumerTaskInitParams*) pvParameters)->consumer_id;

	
	/* Create task */
	if(
		xTaskCreate(
					prvConsumerTask,
					(portCHAR *)"prvConsumerTask",
					configCONSUMER_STACK_SIZE,
					(void*)taskParams, 
					tskCONSUMER_PRIORITY, 
					NULL
					) 
		!= pdTRUE)
	{
		/* Task not created.  Return fail */
		return(1);
	}

	/* Return success */
	return(0);
}

