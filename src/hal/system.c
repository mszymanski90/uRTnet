/*
 *
 *  inc/hal/system.h - Hardware abstraction layer- system module
 *
 *  Copyright (C) 2012  	Maciej Szymañski <mszymanski90@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

#include "inc/hal/system.h"

/*!
 * 	Ustawia czêstotliwosc taktowania rdzenia na 50MHz
 */
void system_init(void)
{
	SysCtlClockSet(SYSCTL_SYSDIV_2 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
						   SYSCTL_XTAL_8MHZ);
}

/*!
 *	Zwraca czêstotliwosc taktowania rdzenia w MHz
 */
uint32_t system_clock_value_in_mhz(void)
{
	return SysCtlClockGet()/1000000;
}
