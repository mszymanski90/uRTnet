/*
 * debug.c
 *
 *  Created on: 13-09-2012
 *      Author: Maciej
 */

#include "debug/debug.h"

uint8_t			debug_ch[N_ch];
uint32_t 		debug_l_int[N_l_int];
uint64_t		debug_ll_int[N_l_int];


void InitDebug(void)
{
	int16_t i;
	for(i=0; i<N_ch; i++) debug_ch[i]=0;
	for(i=0; i<N_l_int; i++) debug_l_int[i]=0;
	for(i=0; i<N_l_int; i++) debug_ll_int[i]=0;
}
