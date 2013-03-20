/*
 *
 *  inc/data_types.h - basic variable types
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

#ifndef DATA_TYPES_H_
#define DATA_TYPES_H_

/*!
 * \file Zawiera redefinicje podstawowych typów danych dla rdzenia ARM Cortex M3.
 * Implementuj¹c HAL dla innego rdzenia, nale¿y odpowiednio zmodyfikowac
 * ten plik.
 * Nazwa uint8_t oznacza 8-bitow¹ zmienn¹ sta³oprzecinkow¹ typu unsigned
 * (st¹d przedrostek "u").
 */

/*!
 * \brief integer 8 bit bez znaku
 */
typedef unsigned char 				uint8_t;

/*!
 * \brief integer 16 bit bez znaku
 */
typedef unsigned short int			uint16_t;

/*!
 * \brief integer 32 bit bez znaku
 */
typedef unsigned long int			uint32_t;

/*!
 * \brief integer 64 bit bez znaku
 */
typedef unsigned long long int		uint64_t;


/*!
 * \brief integer 8 bit ze znakiem
 */
typedef char						int8_t;

/*!
 * \brief integer 16 bit ze znakiem
 */
typedef short int					int16_t;

/*!
 * \brief integer 32 bit ze znakiem
 */
typedef long						int32_t;

/*!
 * \brief integer 64 bit ze znakiem
 */
typedef long long					int64_t;

#endif /* DATA_TYPES_H_ */
