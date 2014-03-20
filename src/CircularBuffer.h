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

/*
 * CircularBuffer.h
 *
 *  Created on: Nov 2, 2012
 *      Author: jon
 */

#ifndef CIRCULARBUFFER_H_
#define CIRCULARBUFFER_H_

/* Kernel includes. */
#include "FreeRTOS.h"
#include "semphr.h"

typedef struct CircularBuffer
{
	char* buffer; // TODO: this should be portBASE_TYPE
	unsigned int size;
	unsigned int head;
	unsigned int tail;
	size_t length;
} CircularBuffer;

typedef struct CircularBufferLockable
{
	xSemaphoreHandle lock_mutex;
	char* buffer; // TODO: this should be portBASE_TYPE
	unsigned int size;
	unsigned int head;
	unsigned int tail;
	size_t length;

} CircularBufferLockable;

int CircularBufferWrite(CircularBuffer *c, unsigned char *dataIn, unsigned int length);
int CircularBufferRead(CircularBuffer *c, unsigned char *dataOut, unsigned int length);

int CircularBufferWriteLockable(CircularBufferLockable *c, unsigned char *dataIn, unsigned int length);
int CircularBufferReadLockable(CircularBufferLockable *c, unsigned char *dataOut, unsigned int length);

#endif /* CIRCULARBUFFER_H_ */
