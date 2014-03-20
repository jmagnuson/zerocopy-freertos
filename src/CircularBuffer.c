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
 * CircularBuffer.c
 *
 *  Created on: Dec 15, 2012
 *      Author: jon
 */

/* Application includes. */
#include "CircularBuffer.h"

/******************************************************************************
	Non-thread-safe routines
******************************************************************************/
int CircularBufferWrite(CircularBuffer *c, unsigned char *dataIn, unsigned int length)
{
	if (c->size >= (c->length + length)){
		// buffer won't overflow

		if ((c->head + length >= c->size)){
			// rollover
			int bytesToLast = c->size - c->head;
			int bytesRemainder = length - bytesToLast;

			memcpy(c->buffer+c->head, dataIn, bytesToLast);
			memcpy(c->buffer, dataIn+bytesToLast, bytesRemainder);

			c->head = bytesRemainder;
			c->length += length;
		}
		else {
			// no rollover
			memcpy(c->buffer+c->head, dataIn, length);
			c->head += length;
			c->length += length;
		}

		return length;
	}
	else {
		// buffer would overflow

		return 0; // TODO: could just write whatever is left of the buffer and return that
	}
}

int CircularBufferRead(CircularBuffer *c, unsigned char *dataOut, unsigned int length)
{

	if (c->length >= length){
		if ((c->tail + length >= c->size)){
			// rollover
			int bytesToLast = c->size - c->tail;
			int bytesRemainder = length - bytesToLast;

			if (dataOut != 0)
			{
				memcpy(dataOut, c->buffer+c->tail, bytesToLast);
				memcpy(dataOut+bytesToLast, c->buffer, bytesRemainder);
			}
			c->tail = bytesRemainder;
			c->length -= length;
		}
		else {
			// no rollover
			if (dataOut != 0)
			{
				memcpy(dataOut, c->buffer+c->tail, length);
			}
			c->tail += length;
			c->length -= length;
		}

		return length;
	}
	else {
		// want to read more bytes than there are
		return 0;
	}
}


/******************************************************************************
	Thread-safe routines
******************************************************************************/
int CircularBufferWriteLockable(CircularBufferLockable *c, unsigned char *dataIn, unsigned int length)
{

	int result = 0;

	// TODO: For P-C, should not need mutex just for reading c->length, only writing, create CircularBufferPC (reserve CBLockable for multi-write/multi-read)
	if (xSemaphoreTake( c->lock_mutex, portMAX_DELAY ) != pdPASS )
	{
		// Return failure
		return -1;
	}

	if (c->size >= (c->length + length)){
		// buffer won't overflow

		if ((c->head + length >= c->size)){
			// rollover
			int bytesToLast = c->size - c->head;
			int bytesRemainder = length - bytesToLast;

			memcpy(c->buffer+c->head, dataIn, bytesToLast);
			memcpy(c->buffer, dataIn+bytesToLast, bytesRemainder);

			c->head = bytesRemainder;
			c->length += length;
		}
		else {
			// no rollover
			memcpy(c->buffer+c->head, dataIn, length);
			c->head += length;
			c->length += length;
		}

		result = length;
	}
	/* Result is already 0
	else {
		// buffer would overflow

		return 0; // TODO: could just write whatever is left of the buffer and return that
	}
	*/

	xSemaphoreGive( c->lock_mutex );

	return result;
}

int CircularBufferReadLockable(CircularBufferLockable *c, unsigned char *dataOut, unsigned int length){

	int result = 0;

	// TODO: For P-C, should not need mutex just for reading c->length, only writing, create CircularBufferPC (reserve CBLockable for multi-write/multi-read)
	if (xSemaphoreTake( c->lock_mutex, portMAX_DELAY ) != pdPASS )
	{
		// Return failure
		return -1;
	}

	/* result is already 0
	if (c->length < length){
		// If requested length is more than availabe, return early
		result = 0;
	}
	*/

	if (c->length >= length)
	{
		if ((c->tail + length >= c->size)){
			// rollover
			int bytesToLast = c->size - c->tail;
			int bytesRemainder = length - bytesToLast;

			if (dataOut != 0)
			{
				memcpy(dataOut, c->buffer+c->tail, bytesToLast);
				memcpy(dataOut+bytesToLast, c->buffer, bytesRemainder);
			}
			c->tail = bytesRemainder;
			c->length -= length;

		}
		else {
			// no rollover

			if (dataOut != 0)
			{
				memcpy(dataOut, c->buffer+c->tail, length);
			}
			c->tail += length;
			c->length -= length;

		}

		result = length;
	}


	xSemaphoreGive( c->lock_mutex );

	return result;
}
