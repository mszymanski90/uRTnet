/*
 *
 *  inc/hal/ethernet.h - Hardware abstraction layer- ethernet module
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

#ifndef ETH_INTERFACE_H_
#define ETH_INTERFACE_H_

#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ethernet.h"
#include "driverlib/gpio.h"
#include "driverlib/flash.h"
#include "driverlib/ethernet.h"
#include "driverlib/interrupt.h"

#include "inc/uRTnet/uRTnet_module.h"
#include "inc/uRTnet/rtmac.h"
#include "inc/mac_addr_type.h"

#include "data_types.h"

#include "config/uRTnet_config.h"


void ethernet_init(void);
uint32_t ethernet_receive_packet(uint8_t* tx_buffer);
void ethernet_send_packet(uint8_t* tx_buffer, int32_t data_size);
void ethernet_get_mac_addr(MAC_addr_t* target);

void ethernet_int_handler(void);

#endif /* ETHERNET_H_ */
