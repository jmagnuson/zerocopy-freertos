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


/* Standard includes. */
#include <stdio.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

/* Application includes. */
#include "CircularBuffer.h"
#include "Barrier.h"
#include "ProducerTaskParams.h"
#include "ConsumerTaskParams.h"
#include "QueueMessage.h"

unsigned long init_consumer_task( void *pvParameters );
unsigned long init_producer_task( void *pvParameters );

#define mainQUEUE_LENGTH 10
#define CBUFF_ARRAY_LENGTH 2048

#define NUM_OF_CONSUMERS 15

void main_app( void )
{
	/* Variable declarations */
	static xProducerTaskInitParams producerTaskInitParams;
	static xConsumerTaskInitParams consumerTaskInitParams[NUM_OF_CONSUMERS];
	QueueHandle_t xQueueArray[NUM_OF_CONSUMERS];
	static portBASE_TYPE circular_buffer_array[CBUFF_ARRAY_LENGTH];
	static CircularBufferLockable circular_buffer = {NULL, NULL, NULL, NULL, NULL, NULL};

	/* Variable initialization */
	{	
        unsigned int i =0;
		for (i=0; i<NUM_OF_CONSUMERS; i++)
		{
			xQueueArray[i] = xQueueCreate( mainQUEUE_LENGTH, sizeof( QueueMessage ) );
		}
	}
	CircularBufferInitializeLockable(&circular_buffer, &circular_buffer_array[0], CBUFF_ARRAY_LENGTH);
	
	producerTaskInitParams.queue = xQueueArray;
	producerTaskInitParams.circular_buffer = &circular_buffer;
	producerTaskInitParams.consumer_task_count = NUM_OF_CONSUMERS;
	init_producer_task( &producerTaskInitParams );
	
	{
		unsigned int i = 0;
		for (i=0; i<NUM_OF_CONSUMERS; i++)
		{
			consumerTaskInitParams[i].queue = xQueueArray[i];
			consumerTaskInitParams[i].circular_buffer = &circular_buffer;
			consumerTaskInitParams[i].consumer_id = i;
			init_consumer_task( &consumerTaskInitParams[i] );
		}
	}
	
	vTaskStartScheduler();
	
	// Shouldn't get here
	for( ;; );
	
}
