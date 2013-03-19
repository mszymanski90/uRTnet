/*
 *
 *  inc/uRTnet/rtmac.h - RTmac
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

#ifndef RTMAC_H_
#define RTMAC_H_

/*!
 * Prototypy funkcji RTmac
 */

#include "debug.h"

#include "config/uRTnet_config.h"
#include "mac_addr_type.h"

#include "tdma.h"

/*!
 * \brief Inicjalizuje warstwê RTmac
 */
void rtmac_init(uRTnet* module);

/*!
 * \brief Obs³uguje ramkê RTmac
 */
void rtmac_handle_rtmac_frame(uRTnet* module);

/*!
 * \brief Funkcja obs³uguj¹ca zdarzenie odbioru ramki
 */
void rtmac_on_rx_frame(uRTnet* module);

#endif /* RTMAC_H_ */
