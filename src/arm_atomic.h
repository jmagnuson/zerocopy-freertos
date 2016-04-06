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

#ifndef ARM_ATOMIC_H_
#define ARM_ATOMIC_H_


__attribute__( ( always_inline ) ) static inline unsigned int sync_bool_compare_and_swap( volatile unsigned int *ptr, unsigned int oldval, unsigned int newval )
{

    if (__ldrex((void*)ptr) == oldval){

        return ( ~(__strex( newval, (void*)ptr)) ) & 0b1;
    }
    return 0;

}

__attribute__( ( always_inline ) ) static inline unsigned int sync_fetch_and_add( volatile unsigned int *ptr, unsigned int value )
{
    unsigned int result=0;

    while(__strex( (result = __ldrex((void*)ptr) ) + value, (void*)ptr) );

    return result;
}

__attribute__( ( always_inline ) ) static inline unsigned int sync_add_and_fetch( volatile unsigned int *ptr, unsigned int value )
{
    unsigned int result=0;

    while(__strex( (result = (__ldrex((void*)ptr) + value)), (void*)ptr) );

    return result;
}

__attribute__( ( always_inline ) ) static inline unsigned int sync_fetch_and_sub( volatile unsigned int *ptr, unsigned int value )
{
    unsigned int result=0;

    while(__strex( (result = __ldrex((void*)ptr) ) - value, (void*)ptr) );

    return result;
}

__attribute__( ( always_inline ) ) static inline unsigned int sync_sub_and_fetch( volatile unsigned int *ptr, unsigned int value )
{
    unsigned int result=0;

    while(__strex( (result = (__ldrex((void*)ptr) - value)), (void*)ptr) );

    return result;
}

__attribute__( ( always_inline ) ) static inline unsigned int sync_fetch_and_increment( volatile unsigned int *ptr )
{
    return sync_fetch_and_add(ptr, 1);
}

__attribute__( ( always_inline ) ) static inline unsigned int sync_increment_and_fetch( volatile unsigned int *ptr )
{
    return sync_add_and_fetch(ptr, 1);
}

__attribute__( ( always_inline ) ) static inline unsigned int sync_fetch_and_decrement( volatile unsigned int *ptr )
{
    return sync_fetch_and_sub(ptr, 1);
}

__attribute__( ( always_inline ) ) static inline unsigned int sync_decrement_and_fetch( volatile unsigned int *ptr )
{
    return sync_sub_and_fetch(ptr, 1);
}


#endif /* ARM_ATOMIC_H_ */
