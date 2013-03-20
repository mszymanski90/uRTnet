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
 * \file Plik zawiera definicje struktur danych uRTnet_buffer, slots_table_row, oraz
 * uRTnet.
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
 * \brief Wiersz tabeli slotów (slots_table)
 *
 *	Pojedyñczy wiersz zawiera informacjê o konfiguracji jednego
 *	slotu: jego phasing (a), period (b), oraz offset (w [ns]).
 */
typedef struct s_slots_table_row
{
	uint32_t	offset;
	uint16_t		a;
	uint16_t		b;
} slots_table_row;

/*!
 * \brief Modu³ uRTnet
 *
 * Zawiera wszystkie struktury danych potrzebne do dzia³ania uRTnet.
 */
typedef struct uRTnet_data
{

	/*!
	 * \brief Status stosu
	 *
	 * Interpretacja poszególnych bitów zmiennej:
	 * 0- Stos jest w fazie inicjalizacji
	 * 1- Stos jest w fazie kalibracji
	 * 2- Stos czeka na odpowiedŸ na ¿¹danie kalibracji
	 */
	uint8_t							uRTnetStatus;

	/*!
	 * \brief Bufor odbiorczy
	 *
	 * Zawiera w³asciwy bufor z danymi, rozmiar danych w buforze, oraz
	 * status bufora (nie u¿ywany).
	 */
	uRTnet_buffer					rx_buffer;

	/*!
	 * \brief Bufor nadawczy
	 *
	 * Zawiera w³asciwy bufor z danymi, rozmiar danych w buforze, oraz
	 * status bufora.
	 */
	uRTnet_buffer					tx_buffer;

	/*!
	 * \brief W³asny adres MAC
	 */
	MAC_addr_t						uC_MAC_Address;

	/*!
	 * \brief Czas przyjscia ostatniej ramki
	 */
	uint64_t						packet_arrival_time;

	/*!
	 * \brief Aktualny numer cyklu
	 */
	int32_t 						cycle_number;

	/*!
	 * \brief Aktualna pozycja w tabeli slotów
	 *
	 * Wartosc zmiennej to indeks nastêpnego slotu w tabeli slotów.
	 */
	uint8_t 						slot_id;

	/*!
	 * \brief Tabela slotów
	 *
	 * Tabela slotów opisuje konfiguracje slotów przys³uguj¹cych
	 * temu urz¹dzeniu. Mo¿na zmienic t¹ konfiguracjê modyfikuj¹c
	 * t¹ tabelê, oraz jej rozmiar w funkcji tdma_init().
	 */
	slots_table_row					slots_table[CONF_max_slots_tab_size];

	/*!
	 * \brief Ilosc przypisanych slotow
	 *
	 * Liczba slotow przypisanych do tego urz¹dzenia.
	 */
	uint8_t 						slots_table_size;

	/*!
	 * \brief Liczba przebytych cykli inicjalizacji
	 */
	uint8_t							initialization_cycles_count;

	/*!
	 * \brief Liczba przebytych cykli kalibracji
	 */
	uint8_t							calibration_cycles_count;

	/*!
	 * \brief Adres MAC tego urz¹dzenia.
	 */
	MAC_addr_t 						Masters_MAC_Address;

	/*!
	 * \brief Odstêp w cyklach do nades³ania odpowiedzi na ¿¹danie kalibracji
	 *
	 * Liczba okresla ile cykli pozniej po wys³aniu ¿¹dania kalibracji ma przyjsc odpowiedz.
	 * Przy wysy³aniu ¿¹dania kalibracji ta wartosc jest dodawana do aktualnego numeru cykl.
	 * Wynik jest wpisywany do ramki ¿¹dania kalibracji.
	 */
	uint32_t						cfg_reply_cycle_offset;

	/*!
	 * \brief Offset slotu w któym ma przyjsc odpowiedz na ¿¹danie kalibracji
	 *
     * Odstêp czasu od ramki synchronizacji po którym ma przyjsc odpowiedz na ¿¹danie
     * kalibrajcji.
	 */
	uint64_t						cfg_reply_slot_offset;

	/*!
	 * \brief Zaplanowany czas wys³ania ramki synchronizacji
	 *
	 * Zaplanowany czas wys³ania ramki synchronizacji odczytany z ostatniej ramki synchronizacji
	 */
	uint64_t						tdma_trans_sched_time;

	/*!
	 * \brief Zmierzony czas wys³ania ramki synchronizacji
	 *
	 * Zmierzony czas wys³ania ramki synchronizacji odczytany z ostatniej ramki synchronizacji
	 */
	uint64_t						tdma_trans_time_stamp;

	/*!
	 * \brief Ró¿nica miêdzy zegarem lokalnym, a globalnym
	 */
	int64_t							t_offset;

	/*!
	 * \brief Czas przesy³ania ramki
	 *
	 * Czas przesy³ania ramki w nanosekundach. Uzyskiwany w czasie fazy kalibracji.
	 */
	int64_t							t_trans;

	/*!
	 * \brief Czas wys³ania ramki
	 */
	uint64_t						packet_transmission_time;

	/*!
	 * \brief Adres wywo³ania zwrotnego, obs³uguj¹cego przyjscie danych
	 */
	void							(*app_rx_callback)(uint8_t *data_buffer, uint32_t data_buffer_size);

	/*!
	 * \brief Adres wywo³ania zwrotnego, obs³uguj¹cego zwolnienie bufora
	 */
	void							(*app_tx_buf_rdy_callback)(void);

	/*!
	 * \brief Adres funkcji inicjalizuj¹cej timery
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
