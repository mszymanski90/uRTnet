/*
 *
 *  src/uRTnet/rtmac.c - RTmac
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

/*!
 * \file Definicje funkcji RTmac
 */

#include "inc/uRTnet/rtmac.h"

/*!
 * Inicjalizuje zmienn¹ packet_arrival_time, oraz uruchamia inicjalizacjê
 * dyscypliny TDMA.
 */
void rtmac_init(uRTnet* module)
{
	module->packet_arrival_time = 0;

	tdma_init(module);
}

/*!
 * Obs³uguje ramkê RTmac. W procedurze rtmac_on_rx_frame() nastêpuje
 * rozpoznanie typu ramki. Je¿eli jest to ramka typu RTmac, to
 * zostaje uruchomiona procedura rtmac_handle_rtmac_frame().
 * Funkcja rozpoznaje czy ramka nale¿y do dyscypliny TDMA, je¿eli
 * tak, to dalsze przetwarzanie przekazuje funkcji tdma_handle_disc_frame().
 * W innym wypadku ramka nie jest dalej przetwarzana.
 */
void rtmac_handle_rtmac_frame(uRTnet* module)
{
	// check rtmac header version
	if(module->rx_buffer.data[16] == 0x02)
	{
		// check if tunneling flag is set
		if( (module->rx_buffer.data[17] & 0x01) == 0x01 )
		{
			// handle tunneled data- not implemented
		}
		else
		{
			// check if frame is tdma frame (rtmac discipline id == 0x0001)
			if( (module->rx_buffer.data[14] == 0x00) && (module->rx_buffer.data[15] == 0x01) )
			{
				tdma_handle_disc_frame(module);
			}
		}
	}
}

/*!
 * Obs³uguje zdarzenie przyjscia ramki. Zapisuje czas przyjscia ramki,
 * kopiuje dane z FIFO mikrokontrolera. Je¿eli pole Ethertype zawiera
 * wartosc 0x9021 (oznaczenie ramek RTmac), to uruchamiana jest
 * procedura rtmac_handle_rtmac_frame(). Dla ramki z danymi
 * uruchamia app_rx_callback().
 */
void rtmac_on_rx_frame(uRTnet* module)
{
	module->packet_arrival_time = module->hal_timers_get_time_ns();

	module->rx_buffer.size = module->hal_ethernet_receive_packet(module->rx_buffer.data);

	// Packet processing
	if(module->rx_buffer.size > 0)
	{
		// check packet type
		if(module->rx_buffer.data[12] == 0x90 && module->rx_buffer.data[13] == 0x21) rtmac_handle_rtmac_frame(module);
		else
		{
			// normal real-time communication
			if(module->app_rx_callback) module->app_rx_callback(module->rx_buffer.data,
																module->rx_buffer.size);
		}
	}
}


