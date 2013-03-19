/*
 *
 *  inc/hal/timers.h - Hardware abstraction layer- timers module
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

#ifndef TIMER_INTERFACE_H_
#define TIMER_INTERFACE_H_

#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ethernet.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"

#include "inc/uRTnet/tdma.h"
#include "inc/hal/system.h"
#include "inc/hal/data_types.h"
#include "debug/debug.h"

// 									   2^32 - 1
#define CONST_clock_lsb_overflow_value 4294967295
#define CONST_clock_msb_overflow_value 214748365

void timers_init(void);
void timers_set_next_slot_time_ns(uint64_t set_value);
void timers_set_time_ns(uint64_t set_value);
uint64_t timers_get_time_ns(void);

uint64_t clock_value_us(void);
uint64_t ns_time_to_cpu_ticks(uint64_t ns_time);
uint64_t cpu_ticks_to_ns_time(uint64_t cpu_ticks);

void Timer0IntHandler(void);
void Timer1IntHandler(void);

#endif /* TIMER_INTERFACE_H_ */
