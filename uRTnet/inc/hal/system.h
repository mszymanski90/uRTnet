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

#ifndef SYSTEM_INTERFACE_H_
#define SYSTEM_INTERFACE_H_

#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "data_types.h"

/*!
 * 	\brief Ustawia czêstotliwosc taktowania rdzenia na 50MHz
 */
void system_init(void);

/*!
 *	\brief Zwraca czêstotliwosc taktowania rdzenia w MHz
 */
uint32_t system_clock_value_in_mhz(void);

#endif /* SYSTEM_INTERFACE_H_ */
