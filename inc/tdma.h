/*
 *
 *  inc/tdma.h - TDMA
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

#ifndef TDMA_H_
#define TDMA_H_

/*!
 * \file TDMA function prototypes
 */

#include "data_types.h"
#include "config/uRTnet_config.h"
#include "uRTnet_module.h"
#include "debug.h"


/*!
 * \brief Initializes TDMA.
 */
void tdma_init(uRTnet* module);

/*!
 * \brief Changes slot configuration [NOT IMPLEMENTED].
 */
void tdma_set_slots(uint32_t *slots_table, uint16_t slots_table_size);

/*!
 * \brief Returns next slot time (absolute).
 */
uint64_t tdma_time_to_next_slot(uRTnet* module);

/*!
 * \brief Finds cycle number and slot offset for calibration reply.
 */
void tdma_find_reply_slot(uRTnet* module, int32_t *cycle_number, uint64_t *slot_offset);

/*!
 * \brief Finds next owned slot with slot id greater than i, in cycle cn.
 */
uint64_t tdma_slot_offset_in_cycle(uRTnet* module, uint16_t i, int32_t cn);

/*!
 * \brief Handles slot event
 */
void tdma_on_slot(uRTnet* module);

/*!
 * \brief Handles TDMA frame
 */
void tdma_handle_disc_frame(uRTnet* module);

/*!
 * \brief Handles reply calibration frame
 */
void tdma_handle_calibration_reply_frame(uRTnet* module);

/*!
 * \brief Queues frame for sending
 */
void tdma_schedule_frame(uRTnet* module, uint8_t *data_buffer, uint16_t data_size);

/*!
 * \brief Sends calibration request frame
 */
void tdma_send_calibration_request(uRTnet* module);

/*!
 * \brief Loads packet to TX buffer
 */
uint8_t tdma_load_packet_to_buffer(uRTnet* module, uint8_t *data_buffer, uint16_t data_size);

#endif /* TDMA_H_ */
