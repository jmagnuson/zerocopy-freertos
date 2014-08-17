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

/* Application includes. */
#include "barcnt.h"

#if defined ( __GNUC__ )
#define X86_ATOMIC
// TODO: Implement generalized atomic check
#endif

int set_barcnt(barcnt_t *b, unsigned int count)
{
	// Take mutex
	if ( xSemaphoreTake( b->lock_mutex, portMAX_DELAY ) != pdPASS )
	{
		// Return failure
		return -1;
	}
	
	// Set barrier
	b->barrier_count = count;
	
	// Give mutex
	xSemaphoreGive( b->lock_mutex );
	
	// Return barrier value
	return count;
}

int dec_barcnt(barcnt_t *b)
{
	int return_value = -1;

#if defined(X86_ATOMIC)
	// Decrement barrier
	return_value = (int)__sync_sub_and_fetch(&(b->barrier_count), 1);
#else
	// Take mutex
	if ( xSemaphoreTake( b->lock_mutex, portMAX_DELAY ) != pdPASS )
	{
		// Return failure
		return -1;
	}
	
	// Decrement barrier
	return_value = --(b->barrier_count);
	
	// Give mutex
	xSemaphoreGive( b->lock_mutex );
#endif
	
	// Return barrier value
	return return_value;
}
