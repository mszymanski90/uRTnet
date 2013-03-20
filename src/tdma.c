/*
 *
 *  src/uRTnet/tdma.c - TDMA
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
 * \file Definicje funkcji TDMA
 */

#include "inc/uRTnet/tdma.h"

/*!
 * Funkcja inicjalizuje zmienne modu³u TDMA. W tej funkcji
 * wpisywana jest konfiguracja slotów do tabeli slotów.
 */
void tdma_init(uRTnet* module)
{
	module->cycle_number = 0;

	module->slot_id = 0;

	/*!
	 * KONFIGURACJA SLOTÓW
	 *
	 * Sloty musz¹ byc upo¿¹dkowane wg. rosn¹cej wartosci offset.
	 *
	 * a- phasing
	 * b- period
	 * offset- odstêp w ns od ramki synchronizacji
	 */
	module->slots_table[0].a = 1;
	module->slots_table[0].b = 1;
	module->slots_table[0].offset = 200000;

	module->slots_table[1].a = 1;
	module->slots_table[1].b = 2;
	module->slots_table[1].offset = 600000;

	module->slots_table_size = 2;
	/*!
	 * KONFIGURACJA SLOTÓW
	 */


	module->initialization_cycles_count = 0;
	module->calibration_cycles_count = 0;

	module->packet_transmission_time = 0;


	module->Masters_MAC_Address.addr[0] = CONF_masters_MAC_0;
	module->Masters_MAC_Address.addr[1] = CONF_masters_MAC_1;
	module->Masters_MAC_Address.addr[2] = CONF_masters_MAC_2;
	module->Masters_MAC_Address.addr[3] = CONF_masters_MAC_3;
	module->Masters_MAC_Address.addr[4] = CONF_masters_MAC_4;
	module->Masters_MAC_Address.addr[5] = CONF_masters_MAC_5;

	module->cfg_reply_cycle_offset = CONF_reply_cycle_offset;

	module->tdma_trans_sched_time = 0;
	module->tdma_trans_time_stamp = 0;

	module->t_offset = 0;
	module->t_trans = 0;
	module->packet_transmission_time = 0;
}

void tdma_set_slots(uint32_t *slots_table, uint16_t slots_table_size)
{
	/*!
	 * NIE ZAIMPLEMENTOWANO
	 */
}


/*!
 * Funkcja szuka nastêpnego posiadanego slotu w cyklu.
 * \retval 0 - je¿eli nie ma posiadanych slotów w cyklu,
 * \retval x - absolutny czas pojawienia siê nastêpnego posiadanego slotu
 */
uint64_t tdma_time_to_next_slot(uRTnet* module)
{
	// find next slot in cycle
	while( module->slots_table[module->slot_id].b != 1 &&
			module->slots_table[module->slot_id].a != ((module->cycle_number+1) %
						module->slots_table[module->slot_id].b) )
	{
		if(module->slot_id >= module->slots_table_size)
		{
			// no more slots in this cycle
			module->slot_id=0;
			return 0;
		}
		module->slot_id++;
	}
	// success
	return module->tdma_trans_sched_time + module->slots_table[module->slot_id].offset;
}

/*!
 * Szuka slotu i cyklu w którym mo¿e przyjsc odpowiedz na ¿¹danie kalibracji.
 * \param *cycle_number - adres zmiennej w której ma zostac zapisany znaleziony numer cyklu
 * \param *slot_offset - adres zmiennej, w której ma zostac zapisany offset znalezionego slotu
 */
void tdma_find_reply_slot(uRTnet* module, int32_t *cycle_number, uint64_t *slot_offset)
{
	int32_t		cn;
	uint16_t 	i;

	// start search from next cycle onward
	cn = module->cycle_number+4;
	i = module->slot_id;

	while( !((*slot_offset) = tdma_slot_offset_in_cycle(module, i, cn)) )
	{
		i=0;
		cn++;
	}

	(*cycle_number) = cn;
}

/*!
 * Przeszukuje tabelê slotów od indeksu i. Zwraca offset pierwszego slotu,
 * który nale¿y do tego urz¹dzenia w cyklu cn.
 * \retval 0 - brak takiego slotu w cyklu cn
 * \retval x - offset w nanosekundach
 */
uint64_t tdma_slot_offset_in_cycle(uRTnet* module, uint16_t i, int32_t cn)
{
	while( module->slots_table[i].b != 1 &&
			module->slots_table[i].a != ((cn+1) % module->slots_table[i].b) )
	{
		if(i >= module->slots_table_size)
		{
			return 0;
		}
		i++;
	}
	return module->slots_table[i].offset;
}


/*!
 * Obs³uguje zdarzenie slotu. Nastawia czas przyjscia nastêpnego slotu.
 * Sprawdza czy w buforze jest ramka do wys³ania, je¿eli tak to
 * jest ona wysy³ana. Ramka ¿¹dania kalibracji jest uzupe³niana o czas
 * wys³ania. Wywo³uje app_tx_buf_rdy_callback po opró¿nieniu bufora.
 */
void tdma_on_slot(uRTnet* module)
{
	module->slot_id++;
	module->hal_timers_set_next_slot_time_ns(tdma_time_to_next_slot(module));

	/* there is a frame in buffer to xmit */
	if(module->tx_buffer.status & TXBUF_FR_IN_BUF)
	{

		/* this is a tdma calibration request frame */
		if(module->tx_buffer.status & TXBUF_CALIB_FR)
		{
			module->packet_transmission_time = module->hal_timers_get_time_ns();

			// write transmission time stamp to buffer
			module->tx_buffer.data[22] = (module->packet_transmission_time >> 56);
			module->tx_buffer.data[23] = (module->packet_transmission_time >> 48);
			module->tx_buffer.data[24] = (module->packet_transmission_time >> 40);
			module->tx_buffer.data[25] = (module->packet_transmission_time >> 32);
			module->tx_buffer.data[26] = (module->packet_transmission_time >> 24);
			module->tx_buffer.data[27] = (module->packet_transmission_time >> 16);
			module->tx_buffer.data[28] = (module->packet_transmission_time >> 8);
			module->tx_buffer.data[29] = (module->packet_transmission_time);
		}

		// send packet
		module->hal_ethernet_send_packet(module->tx_buffer.data, module->tx_buffer.size);

		module->tx_buffer.status &= ~TXBUF_FR_IN_BUF;

		module->tx_buffer.status &= ~TXBUF_CALIB_FR;

		if(module->app_tx_buf_rdy_callback != 0) module->app_tx_buf_rdy_callback();
	}
}

/*!
 * Obs³uguje ramkê TDMA, w szczególnosci ramkê synchronizacji
 */
void tdma_handle_disc_frame(uRTnet* module)
{
	uint64_t param = 0;
	// check tdma version
	if(module->rx_buffer.data[18] == 0x02 && module->rx_buffer.data[19] == 0x01)
	{
		// if tdma synchronization frame
		if(module->rx_buffer.data[20] == 0x00 && module->rx_buffer.data[21] == 0x00)
		{
			// read cycle number
			module->cycle_number = 	(module->rx_buffer.data[22] << 24) +
									(module->rx_buffer.data[23] << 16) +
									(module->rx_buffer.data[24] << 8) +
									module->rx_buffer.data[25];

			// read transmission time stamp and scheduled transmission time

			// READ TIME STAMP ==================================================
			module->tdma_trans_time_stamp = (module->rx_buffer.data[26] << 24) |
											(module->rx_buffer.data[27] << 16) |
											(module->rx_buffer.data[28] << 8) |
											(module->rx_buffer.data[29]);

			module->tdma_trans_time_stamp = (uint64_t) module->tdma_trans_time_stamp << 32;

			module->tdma_trans_time_stamp |= 0x00000000ffffffff &
										(	(module->rx_buffer.data[30] << 24) |
											(module->rx_buffer.data[31] << 16) |
											(module->rx_buffer.data[32] << 8) |
											(module->rx_buffer.data[33])   );

			// READ TIME STAMP ==================================================
			// READ SCHEDULED TIME ==============================================
			module->tdma_trans_sched_time = (module->rx_buffer.data[34] << 24) |
											(module->rx_buffer.data[35] << 16) |
											(module->rx_buffer.data[36] << 8) |
											(module->rx_buffer.data[37]);

			module->tdma_trans_sched_time = ((uint64_t) module->tdma_trans_sched_time) << 32;

			module->tdma_trans_sched_time |= 0x00000000ffffffff &
										(	(module->rx_buffer.data[38] << 24) |
											(module->rx_buffer.data[39] << 16) |
											(module->rx_buffer.data[40] << 8) |
											(module->rx_buffer.data[41])  );
			// READ SCHEDULED TIME ==============================================

			// Synchronization frame marks beginning of a new cycle
			module->slot_id = 0;

			// Calculate t_offset, t_trans comes from calibration phase
			module->t_offset = (module->tdma_trans_time_stamp + module->t_trans) -
								module->packet_arrival_time;

			if(module->uRTnetStatus & INIT_PHASE)
			{
				module->t_offset = 0;
				module->initialization_cycles_count++;
				if(module->initialization_cycles_count >= CONF_max_initialization_cycles)
				{
					module->uRTnetStatus &= ~INIT_PHASE;
					module->uRTnetStatus |= CALIB_PHASE;
				}
			}

			param = module->hal_timers_get_time_ns() + module->t_offset + (module->hal_timers_get_time_ns() - module->packet_arrival_time);

			module->hal_timers_set_time_ns((uint64_t) param);

			module->hal_timers_set_next_slot_time_ns(tdma_time_to_next_slot(module));


			if(module->uRTnetStatus & (CALIB_PHASE))
			{
				if( !(module->uRTnetStatus & (WAITING_FOR_CALIB_RPLY)) )
				{
					module->uRTnetStatus |= (WAITING_FOR_CALIB_RPLY);
					tdma_send_calibration_request(module);
					module->tx_buffer.status |= TXBUF_CALIB_FR;
				}
			}


		}
		else
		{
			if(module->rx_buffer.data[20] == 0x00 && module->rx_buffer.data[21] == 0x11)
			{
				tdma_handle_calibration_reply_frame(module);
			}
		}
	}
}


/*!
 * Odczytuje dane z ramki odpowiedzi na ¿¹danie kalibracji, oblicza
 * now¹ wartosc zmiennej t_trans.
 */
void tdma_handle_calibration_reply_frame(uRTnet* module)
{
	uint64_t 			request_transmission_time = 0;
	uint64_t 			reception_time_stamp = 0;
	uint64_t 			time_stamp = 0;


	module->calibration_cycles_count++;

	// allow sending frames
	module->uRTnetStatus &= ~WAITING_FOR_CALIB_RPLY;

	// READ REQUEST TRANSMISSION TIME ===================================
	request_transmission_time = (module->rx_buffer.data[22] << 24) |
									(module->rx_buffer.data[23] << 16) |
									(module->rx_buffer.data[24] << 8) |
									(module->rx_buffer.data[25]);

	request_transmission_time = (uint64_t) request_transmission_time << 32;

	request_transmission_time |= 0x00000000ffffffff &
								(	(module->rx_buffer.data[26] << 24) |
									(module->rx_buffer.data[27] << 16) |
									(module->rx_buffer.data[28] << 8) |
									(module->rx_buffer.data[29])   );

	// READ REQUEST TRANSMISSION TIME ===================================
	// READ RECEPTION TIME STAMP ========================================
	reception_time_stamp = (module->rx_buffer.data[30] << 24) |
									(module->rx_buffer.data[31] << 16) |
									(module->rx_buffer.data[32] << 8) |
									(module->rx_buffer.data[33]);

	reception_time_stamp = (uint64_t) reception_time_stamp << 32;

	reception_time_stamp |= 0x00000000ffffffff &
								(	(module->rx_buffer.data[34] << 24) |
									(module->rx_buffer.data[35] << 16) |
									(module->rx_buffer.data[36] << 8) |
									(module->rx_buffer.data[37])   );

	// READ RECEPTION TIME STAMP ========================================
	// READ TIME STAMP ==================================================
	time_stamp = (module->rx_buffer.data[38] << 24) |
					(module->rx_buffer.data[39] << 16) |
					(module->rx_buffer.data[40] << 8) |
					(module->rx_buffer.data[41]);

	time_stamp = (uint64_t) time_stamp << 32;

	time_stamp |= 0x00000000ffffffff &
				(	(module->rx_buffer.data[42] << 24) |
					(module->rx_buffer.data[43] << 16) |
					(module->rx_buffer.data[44] << 8) |
					(module->rx_buffer.data[45])   );

	// READ TIME STAMP ==================================================

	if(module->uRTnetStatus & CALIB_PHASE)
	{
		module->t_trans = ( (module->calibration_cycles_count-1) * module->t_trans +
				( ((module->packet_arrival_time - request_transmission_time) -
				(time_stamp - reception_time_stamp)) )/2 ) / module->calibration_cycles_count;
	}

	if(module->calibration_cycles_count >= CONF_max_calibration_cycles)
				module->uRTnetStatus &= ~CALIB_PHASE;
}


/*!
 * Umieszcza dane w buforze nadawczym, ustawia status bufora na zajêty
 */
void tdma_schedule_frame(uRTnet* module, uint8_t *data_buffer, uint16_t data_size)
{
	tdma_load_packet_to_buffer(module, data_buffer, data_size);

	module->tx_buffer.status |= TXBUF_FR_IN_BUF;
}

/*!
 * Buduje ramkê ¿¹dania kalibracji, wpisuj¹c odpowiednie dane do stworzonego bufora.
 * Nastêpnie wywo³ywana jest funkcja tdma_schedule_frame().
 */
void tdma_send_calibration_request(uRTnet* module)
{
	uint64_t tmp_reply_slot_offset;
	int32_t tmp_reply_cycle_number;

	// probna ramka recznie wpisana
	uint8_t buf[] =
	{
			0x00, 0x1b, 0x11, 0x63,
			0x00, 0x00, 0xff, 0xff,
			0xff, 0xff, 0xff, 0xff,
			0x90, 0x21, 0x00, 0x01,
			0x02, 0x00, 0x02, 0x01,
			0x00, 0x10, 0xff, 0xff,
			0xff, 0xff, 0xff, 0xff,
			0xff, 0xff, 0xaa, 0xaa,
			0xaa, 0xaa, 0xcc, 0xcc,
			0xcc, 0xcc, 0xcc, 0xcc,
			0xcc, 0xcc
	};

	// do poprawki
	tdma_find_reply_slot(module, &tmp_reply_cycle_number, &tmp_reply_slot_offset);

	// address this frame to master node
	buf[0] = module->Masters_MAC_Address.addr[0];
	buf[1] = module->Masters_MAC_Address.addr[1];
	buf[2] = module->Masters_MAC_Address.addr[2];
	buf[3] = module->Masters_MAC_Address.addr[3];
	buf[4] = module->Masters_MAC_Address.addr[4];
	buf[5] = module->Masters_MAC_Address.addr[5];

	// write sender address to buffer
	buf[6] = module->uC_MAC_Address.addr[0];
	buf[7] = module->uC_MAC_Address.addr[1];
	buf[8] = module->uC_MAC_Address.addr[2];
	buf[9] = module->uC_MAC_Address.addr[3];
	buf[10] = module->uC_MAC_Address.addr[4];
	buf[11] = module->uC_MAC_Address.addr[5];

	// write frame type to buffer (0x9021)
	buf[12] = 0x90;
	buf[13] = 0x21;

	// write RTmac discipline id to buffer
	buf[14] = 0x00;
	buf[15] = 0x01;

	// write RTmac header version to buffer
	buf[16] = 0x02;
	buf[17] = 0x00;

	// write TDMA discipline version to buffer
	buf[18] = 0x02;
	buf[19] = 0x01;

	// write TDMA frame id to buffer
	buf[20] = 0x00;
	buf[21] = 0x10;

	// write reply cycle number to buffer
	buf[30] = (tmp_reply_cycle_number >> 24);
	buf[31] = (tmp_reply_cycle_number >> 16);
	buf[32] = (tmp_reply_cycle_number >> 8);
	buf[33] = (tmp_reply_cycle_number);

	// write reply slot offset to buffer (8 bytes)
	buf[34] = (tmp_reply_slot_offset >> 56);
	buf[35] = (tmp_reply_slot_offset >> 48);
	buf[36] = (tmp_reply_slot_offset >> 40);
	buf[37] = (tmp_reply_slot_offset >> 32);
	buf[38] = (tmp_reply_slot_offset >> 24);
	buf[39] = (tmp_reply_slot_offset >> 16);
	buf[40] = (tmp_reply_slot_offset >> 8);
	buf[41] = (tmp_reply_slot_offset);


	tdma_schedule_frame(module, buf, 42);
}

/*!
 * Kopiuje dane z data_buffer do bufora nadawczego.
 * \param data_buffer - bufor z danymi do wys³ania,
 * \param data_size - rozmiar danych w buforze,
 * \retval 1 - rozmiar danych nie przekracza maksymalnego rozmiaru ramki,
 * \retval 0 - dane nie zosta³y skopiowane, rozmiar danych przekracza dopuszczalny rozmiar ramki,
 */
uint8_t tdma_load_packet_to_buffer(uRTnet* module, uint8_t *data_buffer, uint16_t data_size)
{
	uint16_t i=0;

	if(data_size < CONF_eth_buffer_LENGTH+2)
	{
		module->tx_buffer.size = data_size;
		for(i=0; i < data_size; i++)
		{
			module->tx_buffer.data[i] = data_buffer[i];
		}

		return 1;
	}
	else return 0;
}
