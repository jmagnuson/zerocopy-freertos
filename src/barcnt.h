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

#ifndef BARRIER_H_
#define BARRIER_H_

/* Kernel includes. */
#include "FreeRTOS.h"
#include "semphr.h"

typedef struct Barrier
{
	xSemaphoreHandle lock_mutex;
	unsigned int barrier_count;
} Barrier;

int set_barrier(Barrier *b, unsigned int count);
int decrement_barrier(Barrier *b);

#endif
