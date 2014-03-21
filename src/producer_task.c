/****************************************************************************
*
* Copyright (C) 2014
* Written by Jon Magnuson, (my.name at google's mail service)
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

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "portable.h"

/* Application includes. */
#include "ProducerTaskParams.h"
#include "CircularBuffer.h"
#include "Barrier.h"
#include "QueueMessage.h"

//*****************************************************************************
//
// The stack size for the task
//
//*****************************************************************************
#define configPRODUCER_STACK_SIZE        configMINIMAL_STACK_SIZE // Stack size in words

//*****************************************************************************
//
// The stack size for the task
//
//*****************************************************************************
#define tskPRODUCER_PRIORITY        (tskIDLE_PRIORITY + 1) // Stack size in words

//*****************************************************************************
//
// Main task
//
//*****************************************************************************
static void
prvProducerTask(void *pvParameters)
{
	/* Variable declarations */
	QueueHandle_t *queue = NULL;
	Barrier *barrier_array = NULL;
	CircularBufferLockable* circular_buffer = 0;
	unsigned int consumer_task_count = 0;
	QueueMessage message = {NULL, NULL, NULL};
	portBASE_TYPE local_buffer[256];
	portBASE_TYPE local_buffer_length = 0;
	portBASE_TYPE local_counter = 0;
	unsigned int i = 0;
	/* Variable initialization */
	queue = ((xProducerTaskParams*)pvParameters)->queue;
	circular_buffer = ((xProducerTaskParams*)pvParameters)->circular_buffer;
	consumer_task_count = ((xProducerTaskParams*)pvParameters)->consumer_task_count;
	barrier_array = ((xProducerTaskParams*)pvParameters)->barrier_array;

	for (;;)
	{
		__asm( "nop" );

		// Wait 1 second
		vTaskDelay(1000 / portTICK_RATE_MS);

		sprintf(local_buffer, "%d", local_counter++);
		local_buffer_length = strlen(local_buffer);

		// Send message
		if (CircularBufferWriteLockable(circular_buffer, local_buffer,local_buffer_length) > 0)
		{
			message.buffer_ptr = (circular_buffer->buffer+circular_buffer->tail);
			message.buffer_len = local_buffer_length;
			message.consumer_count_barrier = &barrier_array[local_counter%10];
			message.consumer_count_barrier->barrier_count = consumer_task_count;

			for (i=0; i<consumer_task_count; i++)
			{
				if (xQueueSendToBack(queue[i], &message, (portTickType) 100) != pdPASS)
				{
					// Consumer doesn't get the data.  Reflect this in barrier.
					portENTER_CRITICAL();
					((barrier_array[local_counter%10]).barrier_count)--;
					portEXIT_CRITICAL();
				}
			}
		}
	}
	
}

//*****************************************************************************
//
// Init task
//
//*****************************************************************************
extern unsigned long
init_producer_task(void *pvParameters)
{
	xProducerTaskParams *taskParams = ( xProducerTaskParams * ) pvPortMalloc( sizeof( xProducerTaskParams ) );

	taskParams->queue = ((xProducerTaskInitParams*) pvParameters)->queue;
	taskParams->circular_buffer = ((xProducerTaskInitParams*) pvParameters)->circular_buffer;
	taskParams->consumer_task_count = ((xProducerTaskInitParams*) pvParameters)->consumer_task_count;

	Barrier *barrier_array = pvPortMalloc( 10 * sizeof( Barrier ) );
	{ // Scope out the counter
		volatile unsigned int local_counter = 0;
		for (local_counter=0; local_counter<10; local_counter++)
		{
			(barrier_array[local_counter]).lock_mutex = xSemaphoreCreateMutex();
		}
	}
	taskParams->barrier_array = barrier_array;
	

	/* Create task */
	if(
		xTaskCreate(
					prvProducerTask,
					(portCHAR *)"prvProducerTask",
					configPRODUCER_STACK_SIZE,
					(void*)taskParams, 
					tskPRODUCER_PRIORITY, 
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

