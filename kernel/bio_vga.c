/*
* Copyright (C) 2014,  Charlie Fyvie-Gauld (Carrotsrc.org)
*
*    This program is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "kernel/sys.h"
#include "kernel/bio_vga.h"
/*
 * Encode a character with code and color
 */
uint16_t biov_encode_char(uint8_t ch, uint8_t color)
{
	uint16_t enc = color;
	enc = (enc << 8) | ch;
	return enc;
}

uint8_t biov_encode_color(enum biov_color fg, enum biov_color bg)
{
	return fg|(bg<<4);
}
