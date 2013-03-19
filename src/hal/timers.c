/*
 *
 *  inc/hal/timers.c - Hardware abstraction layer- timers module
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

#include "inc/hal/timers.h"


extern uRTnet					uRTnet_module;

// trzeba pomiescic przynajmniej 18446744073709551615
// wartosc oznacza [ns], czyli mozemy to podzielic przez 10^3
// przy 10^9 mamy przepelnienie timera
// po odjeciu czesci znajdujacej sie w timerze
// do zapisania zostaje 18446744 (miesci sie w 32-bit)
uint32_t clock_msb;

uint32_t ns_per_tick;

void timers_init(void)
{

	ns_per_tick = 1000 / system_clock_value_in_mhz();

	// Init slot timer
	// Enable TIMER0 Peripheral
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	// Configure TIMER0 as non-periodic
	TimerConfigure(TIMER0_BASE, TIMER_CFG_ONE_SHOT);

	TimerLoadSet(TIMER0_BASE, TIMER_A, ns_time_to_cpu_ticks(200 * 1000));
	IntEnable(INT_TIMER0A);
	TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);



	// Init clock
	clock_msb=0;
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);

	TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);

	// Nastawic timer na 2^32 - 1,
	// wtedy na msb zostaje 214748364,85 ~= 214748365 (wtedy dac przepelnienie msb)
	TimerLoadSet(TIMER1_BASE, TIMER_A, (uint32_t) CONST_clock_lsb_overflow_value);

	IntEnable(INT_TIMER1A);
	TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);

	TimerEnable(TIMER1_BASE, TIMER_A);
}

void timers_set_next_slot_time_ns(uint64_t set_value)
{
	uint64_t value;
	uint64_t clock;

	if(set_value != 0)
	{
		clock = timers_get_time_ns();
		// read clock and calculate relative time

		value = (set_value) - clock;
		// set and start timer
		TimerLoadSet(TIMER0_BASE, TIMER_A, value / ns_per_tick);
		TimerEnable(TIMER0_BASE, TIMER_A);
	}
}

void timers_set_time_ns(uint64_t set_value)
{
	uint32_t param;
	//uint64_t help;
	uint64_t help2;
	//help = timers_get_time_ns();

	set_value = set_value / ns_per_tick;

	param = 0xffffffff - (0x00000000ffffffff & set_value);
	clock_msb = (set_value & 0xffffffff00000000) >> 32;

	TimerLoadSet(TIMER1_BASE, TIMER_A, param);
}

uint64_t timers_get_time_ns(void)
{
	//TimerIntDisable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
	uint64_t tmp = 0;
	uint32_t tmp1 = 0;
	uint32_t tmp2 = 0;

	tmp = clock_msb;
	tmp2 = TimerValueGet(TIMER1_BASE, TIMER_A);

	tmp = tmp << 32;
	tmp1 = (0xffffffff - tmp2);
	tmp |=  tmp1;

	//TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
	return tmp * ns_per_tick;
}

uint64_t ns_time_to_cpu_ticks(uint64_t ns_time)
{
	return (ns_time / ns_per_tick);
}

uint64_t cpu_ticks_to_ns_time(uint64_t cpu_ticks)
{
	return (cpu_ticks * ns_per_tick);
}

void Timer0IntHandler(void)
{
    // Clear the timer interrupt.
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    tdma_on_slot(&uRTnet_module);
}

void Timer1IntHandler(void)
{
    // Clear the timer interrupt.
    TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);

    if(TimerLoadGet(TIMER1_BASE, TIMER_A) != CONST_clock_lsb_overflow_value)
    {
    	TimerLoadSet(TIMER1_BASE, TIMER_A, (uint32_t) CONST_clock_lsb_overflow_value);
    }

    clock_msb++;

    if(clock_msb > CONST_clock_msb_overflow_value) clock_msb=0;
}

