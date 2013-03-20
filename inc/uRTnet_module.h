/*
 *
 *  inc/uRTnet_module.h - uRTnet data structure
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
 * \brief Wiersz tabeli slot�w (slots_table)
 *
 *	Pojedy�czy wiersz zawiera informacj� o konfiguracji jednego
 *	slotu: jego phasing (a), period (b), oraz offset (w [ns]).
 */
typedef struct s_slots_table_row
{
	uint32_t	offset;
	uint16_t		a;
	uint16_t		b;
} slots_table_row;

/*!
 * \brief Modu� uRTnet
 *
 * Zawiera wszystkie struktury danych potrzebne do dzia�ania uRTnet.
 */
typedef struct uRTnet_data
{

	/*!
	 * \brief Status stosu
	 *
	 * Interpretacja poszeg�lnych bit�w zmiennej:
	 * 0- Stos jest w fazie inicjalizacji
	 * 1- Stos jest w fazie kalibracji
	 * 2- Stos czeka na odpowied� na ��danie kalibracji
	 */
	uint8_t							uRTnetStatus;

	/*!
	 * \brief Bufor odbiorczy
	 *
	 * Zawiera w�asciwy bufor z danymi, rozmiar danych w buforze, oraz
	 * status bufora (nie u�ywany).
	 */
	uRTnet_buffer					rx_buffer;

	/*!
	 * \brief Bufor nadawczy
	 *
	 * Zawiera w�asciwy bufor z danymi, rozmiar danych w buforze, oraz
	 * status bufora.
	 */
	uRTnet_buffer					tx_buffer;

	/*!
	 * \brief W�asny adres MAC
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
	 * \brief Aktualna pozycja w tabeli slot�w
	 *
	 * Wartosc zmiennej to indeks nast�pnego slotu w tabeli slot�w.
	 */
	uint8_t 						slot_id;

	/*!
	 * \brief Tabela slot�w
	 *
	 * Tabela slot�w opisuje konfiguracje slot�w przys�uguj�cych
	 * temu urz�dzeniu. Mo�na zmienic t� konfiguracj� modyfikuj�c
	 * t� tabel�, oraz jej rozmiar w funkcji tdma_init().
	 */
	slots_table_row					slots_table[CONF_max_slots_tab_size];

	/*!
	 * \brief Ilosc przypisanych slotow
	 *
	 * Liczba slotow przypisanych do tego urz�dzenia.
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
	 * \brief Adres MAC tego urz�dzenia.
	 */
	MAC_addr_t 						Masters_MAC_Address;

	/*!
	 * \brief Odst�p w cyklach do nades�ania odpowiedzi na ��danie kalibracji
	 *
	 * Liczba okresla ile cykli pozniej po wys�aniu ��dania kalibracji ma przyjsc odpowiedz.
	 * Przy wysy�aniu ��dania kalibracji ta wartosc jest dodawana do aktualnego numeru cykl.
	 * Wynik jest wpisywany do ramki ��dania kalibracji.
	 */
	uint32_t						cfg_reply_cycle_offset;

	/*!
	 * \brief Offset slotu w kt�ym ma przyjsc odpowiedz na ��danie kalibracji
	 *
     * Odst�p czasu od ramki synchronizacji po kt�rym ma przyjsc odpowiedz na ��danie
     * kalibrajcji.
	 */
	uint64_t						cfg_reply_slot_offset;

	/*!
	 * \brief Zaplanowany czas wys�ania ramki synchronizacji
	 *
	 * Zaplanowany czas wys�ania ramki synchronizacji odczytany z ostatniej ramki synchronizacji
	 */
	uint64_t						tdma_trans_sched_time;

	/*!
	 * \brief Zmierzony czas wys�ania ramki synchronizacji
	 *
	 * Zmierzony czas wys�ania ramki synchronizacji odczytany z ostatniej ramki synchronizacji
	 */
	uint64_t						tdma_trans_time_stamp;

	/*!
	 * \brief R�nica mi�dzy zegarem lokalnym, a globalnym
	 */
	int64_t							t_offset;

	/*!
	 * \brief Czas przesy�ania ramki
	 *
	 * Czas przesy�ania ramki w nanosekundach. Uzyskiwany w czasie fazy kalibracji.
	 */
	int64_t							t_trans;

	/*!
	 * \brief Czas wys�ania ramki
	 */
	uint64_t						packet_transmission_time;

	/*!
	 * \brief Adres wywo�ania zwrotnego, obs�uguj�cego przyjscie danych
	 */
	void							(*app_rx_callback)(uint8_t *data_buffer, uint32_t data_buffer_size);

	/*!
	 * \brief Adres wywo�ania zwrotnego, obs�uguj�cego zwolnienie bufora
	 */
	void							(*app_tx_buf_rdy_callback)(void);

	/*!
	 * \brief Adres funkcji inicjalizuj�cej timery
	 */
	void 							(*hal_timers_init)(void);

	/*!
	 * \brief Adres funkcji ustawiaj�cej czas w kt�rym ma pojawic si� posiadany slot
	 */
	void 							(*hal_timers_set_next_slot_time_ns)(uint64_t set_value);

	/*!
	 * \brief Adres funkcji ustawiaj�cej wartosc lokalnego zegara
	 */
	void 							(*hal_timers_set_time_ns)(uint64_t set_value);

	/*!
	 * \brief Adres funkcji zwracaj�cej wartosc lokalnego zegara
	 */
	uint64_t 						(*hal_timers_get_time_ns)(void);

	/*!
	 * \brief Adres funkcji inicjalizuj�cej kontroler Ethernet
	 */
	void 							(*hal_ethernet_init)(void);

	/*!
	 * \brief Adres funkcji odbieraj�cej pakiet z kontrolera Ethernet
	 */
	uint32_t 						(*hal_ethernet_receive_packet)(uint8_t* tx_buffer);

	/*!
	 * \brief Adres funkcji wysy�aj�cej pakiet przez kontroler Ethernet
	 */
	void 							(*hal_ethernet_send_packet)(uint8_t* tx_buffer, long int data_size);

	/*!
	 * \brief Adres funkcji odczytuj�cej z pami�ci adres MAC mikrokontrolera i
	 * wpisuj�cej go do target.
	 */
	void							(*hal_ethernet_get_mac_addr)(MAC_addr_t* target);

} uRTnet;

#endif /* URTNET_MODULE_H_ */
