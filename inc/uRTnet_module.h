/*
 *
 *  inc/uRTnet_module.h - uRTnet data structure
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

#ifndef URTNET_MODULE_H_
#define URTNET_MODULE_H_

/*!
 * \file Definitions of data structures: uRTnet_buffer, slots_table_row and uRTnet.
 */

#include "data_types.h"
#include "mac_addr_type.h"
#include "config/uRTnet_config.h"

#define NORMAL_STATUS 0x00
#define INIT_PHASE 0x01
#define CALIB_PHASE 0x02
#define WAITING_FOR_CALIB_RPLY 0x04

#define TXBUF_READY_FOR_DATA 0x00
#define TXBUF_FR_IN_BUF 0x01
#define TXBUF_CALIB_FR 0x02

/*

 *
 * tx_buffer.status - TX Buffer Status register
 *
 * bit id		|	(if bit is 1)
 * -----------------------------------------------
 * 0- 			|	There is a frame in buffer
 * 1-			|	Frame in buffer is a calibration reguest frame
 */

typedef struct urtnet_buffer
{
	uint8_t 							data[CONF_eth_buffer_LENGTH+2];
	uint8_t 							status;
	int32_t								size;
} uRTnet_buffer;

/*
 * \brief Configuration of one slot
 *
 *	One slots_table_row contains information on one (owned) slot:
 *	phasing (a), period (b) and offset ([ns]).
 */
typedef struct s_slots_table_row
{
	uint32_t	offset;
	uint16_t		a;
	uint16_t		b;
} slots_table_row;

/*!
 * \brief uRTnet module
 *
 * Main structure for uRTnet.
 */
typedef struct uRTnet_data
{

	/*!
	 * \brief uRTnet network stack status
	 *
	 * Bits:
	 * 0- initialization phase
	 * 1- kalibration phase
	 * 2- waiting for calibration reply
	 */
	uint8_t							uRTnetStatus;

	/*!
	 * \brief Receive buffer
	 *
	 * Contains actual data buffer, size of data in buffer and (not used) status.
	 */
	uRTnet_buffer					rx_buffer;

	/*!
	 * \brief Transmit buffer
	 *
	 * Contains actual data buffer, size of data in buffer and (not used) status.
	 */
	uRTnet_buffer					tx_buffer;

	/*!
	 * \brief Microcontrollers own MAC adress
	 */
	MAC_addr_t						uC_MAC_Address;

	/*!
	 * \brief Last frame arrival time
	 */
	uint64_t						packet_arrival_time;

	/*!
	 * \brief Current cycle number
	 */
	int32_t 						cycle_number;

	/*!
	 * \brief Position in slots table
	 *
	 * Value represents index of next slot in slots table.
	 */
	uint8_t 						slot_id;

	/*!
	 * \brief Slots table
	 *
	 * Contains slot configuration.
	 */
	slots_table_row					slots_table[CONF_max_slots_tab_size];

	/*!
	 * \brief Number of owned slots
	 */
	uint8_t 						slots_table_size;

	/*!
	 * \brief Number of completed initialization cycles
	 */
	uint8_t							initialization_cycles_count;

	/*!
	 * \brief Number of completed calibration cycles
	 */
	uint8_t							calibration_cycles_count;

	/*!
	 * \brief Master's MAC address.
	 */
	MAC_addr_t 						Masters_MAC_Address;

	/*!
	 * \brief Cycles to receiving calibration reply
	 *
	 * Determines how many cycles later calibration request should be replied. When calibration
	 * request is send, this value is added to actual cycle number. Result is number of cycle
	 * in which calibration reply will come (master sends calibration reply in cycle specified by
	 * result).
	 */
	uint32_t						cfg_reply_cycle_offset;

	/*!
	 * \brief Specifies slot in which calibration reply should be sent.
	 *
     * Offset of slot in which master should sent calibration reply.
	 */
	uint64_t						cfg_reply_slot_offset;

	/*!
	 * \brief Scheduled transmission time of synchronization frame
	 *
	 * Scheduled transmission time of last received synchronization frame. Value is read
	 * from synchronization frame and written in this field.
	 */
	uint64_t						tdma_trans_sched_time;

	/*!
	 * \brief Synchronization frame's time stamp
	 *
	 * Master measures transmission time of synchronization frame and writes result in that frame.
	 * Upon receiving synchronization frame, the value is read and written in this field.
	 */
	uint64_t						tdma_trans_time_stamp;

	/*!
	 * \brief Offset between local and global (master's) clock.
	 */
	int64_t							t_offset;

	/*!
	 * \brief Estimated transmission time.
	 *
	 * Estimated transmission time in nanoseconds. Computed during calibration phase.
	 */
	int64_t							t_trans;

	/*!
	 * \brief Transmitted frame's time stamp
	 */
	uint64_t						packet_transmission_time;

	/*!
	 * \brief Callback handling received data
	 */
	void							(*app_rx_callback)(uint8_t *data_buffer, uint32_t data_buffer_size);

	/*!
	 * \brief Callback handling buffer ready
	 */
	void							(*app_tx_buf_rdy_callback)(void);

	/*!
	 * \brief Callback to HAL, handling timer initialization
	 */
	void 							(*hal_timers_init)(void);

	/*!
	 * \brief Adres funkcji ustawiaj¹cej czas w którym ma pojawic siê posiadany slot
	 */
	void 							(*hal_timers_set_next_slot_time_ns)(uint64_t set_value);

	/*!
	 * \brief Adres funkcji ustawiaj¹cej wartosc lokalnego zegara
	 */
	void 							(*hal_timers_set_time_ns)(uint64_t set_value);

	/*!
	 * \brief Adres funkcji zwracaj¹cej wartosc lokalnego zegara
	 */
	uint64_t 						(*hal_timers_get_time_ns)(void);

	/*!
	 * \brief Adres funkcji inicjalizuj¹cej kontroler Ethernet
	 */
	void 							(*hal_ethernet_init)(void);

	/*!
	 * \brief Adres funkcji odbieraj¹cej pakiet z kontrolera Ethernet
	 */
	uint32_t 						(*hal_ethernet_receive_packet)(uint8_t* tx_buffer);

	/*!
	 * \brief Adres funkcji wysy³aj¹cej pakiet przez kontroler Ethernet
	 */
	void 							(*hal_ethernet_send_packet)(uint8_t* tx_buffer, long int data_size);

	/*!
	 * \brief Adres funkcji odczytuj¹cej z pamiêci adres MAC mikrokontrolera i
	 * wpisuj¹cej go do target.
	 */
	void							(*hal_ethernet_get_mac_addr)(MAC_addr_t* target);

} uRTnet;

#endif /* URTNET_MODULE_H_ */
