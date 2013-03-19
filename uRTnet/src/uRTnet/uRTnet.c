/*
 *
 *  src/uRTnet/uRTnet.c - main library
 *
 *  Copyright (C) 2012  	Maciej Szyma�ski <mszymanski90@gmail.com>
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

/*!
 * \file Definicje funkcji interfejsu aplikacji.
 */

#include "inc/uRTnet/uRTnet.h"

/*!
 * Funkcja przeprowadza inicjalizacj� modu�u uRTnet. Wst�pnie inicjalizuje struktury danych
 * uRTnet, nast�pnie uruchamia timery i kontroler Ethernet.
 * \param module - adres modu�u uRTnet.
 * \param arg_app_rx_callback - adres wywo�ania zwrotnego, obs�uguj�cego przyjscie danych.
 * \param arg_app_tx_buf_rdy_callback - adres wywo�ania zwrotnego, obs�uguj�cego zwolnienie bufora
 * 	nadawczego.
 * \param arg_hal_timers_init - adres funkcji inicjalizuj�cej timery.
 * \param arg_hal_timers_set_next_slot_time_ns - adres funkcji ustawiaj�cej czas w kt�rym
 * 	ma pojawia si� posiadany slot.
 * \param arg_hal_timers_set_time_ns - adres funkcji ustawiaj�cej wartosc lokalnego zegara.
 * \param arg_hal_timers_get_time_ns - adres funkcji zwracaj�cej wartosc lokalnego zegara.
 * \param arg_hal_ethernet_init - adres funkcji inicjalizuj�cej kontroler Ethernet.
 * \param arg_hal_ethernet_receive_packet - adres funkcji odbieraj�cej pakiet z kontrolera Ethernet.
 * \param arg_hal_ethernet_send_packet - adres funkcji wysy�aj�cej pakiet przez kontroler Ethernet.
 * \param arg_hal_ethernet_get_mac_addr - adres funkcji odczytuj�cej z pami�ci adres MAC
 * 	mikrokontrolera i wpisuj�cej go do target.
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
				)
{
	module->app_rx_callback = arg_app_rx_callback;
	module->app_tx_buf_rdy_callback = arg_app_tx_buf_rdy_callback;
	module->hal_timers_init = arg_hal_timers_init;
	module->hal_timers_set_next_slot_time_ns = arg_hal_timers_set_next_slot_time_ns;
	module->hal_timers_set_time_ns = arg_hal_timers_set_time_ns;
	module->hal_timers_get_time_ns = arg_hal_timers_get_time_ns;

	module->hal_ethernet_init = arg_hal_ethernet_init;
	module->hal_ethernet_receive_packet = arg_hal_ethernet_receive_packet;
	module->hal_ethernet_send_packet = arg_hal_ethernet_send_packet;
	module->hal_ethernet_get_mac_addr = arg_hal_ethernet_get_mac_addr;


	module->uRTnetStatus |= INIT_PHASE;

	module->rx_buffer.size = 0;
	module->rx_buffer.status = 0;

	module->tx_buffer.size = 0;
	module->tx_buffer.status = 0;

	module->hal_timers_init();
	module->hal_ethernet_init();
	module->hal_ethernet_get_mac_addr(&(module->uC_MAC_Address));

	rtmac_init(module);

}

/*!
 * Umo�liwia wys�anie danych za posrednictwem stosu uRTnet.
 * \retval 1 - dane zosta�y przekopiowane do bufora nadawczego i zostan� wys�ane po nadejsciu nast�pnego slotu
 * \retval 0 - nie mo�na w tej chwili wysy�ac danych (bufor jest zaj�ty, lub stos nie znajduje si� w
 * 	stanie normalnym).
 */
uint8_t Send(uRTnet* module, uint8_t *data_buffer, uint16_t data_size)
{
	if(module->uRTnetStatus == NORMAL_STATUS && module->tx_buffer.status == TXBUF_READY_FOR_DATA)
	{
		tdma_schedule_frame(module, data_buffer, data_size);
		return 1;
	}
	else return 0;
}






