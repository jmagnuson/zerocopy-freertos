/****************************************************************************
*
* Copyright (C) 2014, Jon Magnuson <my.name at google's mail service>
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

/*
 * ringbuf.h
 *
 *  Created on: Nov 2, 2012
 *      Author: jon
 */

#ifndef RINGBUF_H_
#define RINGBUF_H_

/* Kernel includes. */
#include "FreeRTOS.h"
#include "semphr.h"

typedef struct _ringbuf_t
{
	char* buffer; // TODO: this should be portBASE_TYPE
	unsigned int size;
	unsigned int head;
	unsigned int tail;
	size_t length;
} ringbuf_t;

typedef struct _atomic_ringbuf_t
{
	xSemaphoreHandle lock_mutex;
	char* buffer; // TODO: this should be portBASE_TYPE
	unsigned int size;
	unsigned int head;
	unsigned int tail;
	size_t length;
} atomic_ringbuf_t;

int init_ringbuf(ringbuf_t *c, unsigned char *buffer, unsigned int buffer_size);
int write_ringbuf(ringbuf_t *c, unsigned char *dataIn, unsigned int length);
int read_ringbuf(ringbuf_t *c, unsigned char *dataOut, unsigned int length);

int init_atomic_ringbuf(atomic_ringbuf_t *c, unsigned char *buffer, unsigned int buffer_size);
int write_atomic_ringbuf(atomic_ringbuf_t *c, unsigned char *dataIn, unsigned int length);
int read_atomic_ringbuf(atomic_ringbuf_t *c, unsigned char *dataOut, unsigned int length);

#endif /* CIRCULARBUFFER_H_ */
