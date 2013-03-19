/*
 *
 *  inc/uRTnet/uRTnet.h - main library
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

#ifndef URTNET_H_
#define URTNET_H_

/*!
 * \file Prototypy funkcji interfejsu aplikacji
 */

#include "hal/data_types.h"
#include "uRTnet_module.h"
#include "rtmac.h"
#include "tdma.h"

/*!
 * \brief Inicjalizuje uRTnet
 */
void uRTnet_init(uRTnet* module,
					void (*arg_app_rx_callback)(uint8_t *data_buffer, uint32_t data_buffer_size),
					void (*arg_app_tx_buf_rdy_callback)(void),
					void (*arg_hal_timers_init)(void),
					void (*arg_hal_timers_set_next_slot_time_ns)(uint64_t set_value),
					void (*arg_hal_timers_set_time_ns)(uint64_t set_value),
					uint64_t (*arg_hal_timers_get_time_ns)(void),
					void (*arg_hal_ethernet_init)(void),
					uint32_t (*arg_hal_ethernet_receive_packet)(uint8_t* tx_buffer),
					void (*arg_hal_ethernet_send_packet)(uint8_t* tx_buffer, int32_t data_size),
					void (*arg_hal_ethernet_get_mac_addr)(MAC_addr_t* target)
				);

/*!
 * \brief Wysy³a bufor z danymi
 */
uint8_t Send(uRTnet* module, uint8_t *data_buffer, uint16_t data_buffer_size);

#endif /* URTNET_H_ */
