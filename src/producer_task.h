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

#ifndef PRODUCERTASKPARAMS_H_
#define PRODUCERTASKPARAMS_H_

/* Kernel includes. */
#include "FreeRTOS.h"
#include "queue.h"

/* Application includes. */
#include "barcnt.h"
#include "ringbuf.h"

typedef struct xProducerTaskInitParams {

	QueueHandle_t* queue;
	//barcnt* barrier;
	atomic_ringbuf_t* circular_buffer;
	unsigned int consumer_task_count;
	
} xProducerTaskInitParams;

typedef struct xProducerTaskParams {

	QueueHandle_t* queue;
	barcnt_t* barrier_array;
	atomic_ringbuf_t* circular_buffer;
	unsigned int consumer_task_count;
	
} xProducerTaskParams;

#endif /* CONSUMERTASKPARAMS_H_ */
