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
#ifndef __BIO_TERM_H
#define __BIO_TERM_H

struct biot_terminal {
	uint8_t row;
	uint8_t col;

	uint8_t cr;
	uint8_t cc;

	uint8_t r_off;
	uint8_t dflt_clr;
	uint16_t *buffer;
};

struct biot_terminal biot_init(uint8_t row, uint8_t col, uint8_t clr);
void biot_clr(struct biot_terminal *term);

uint8_t *biot_getbuf(struct biot_terminal *term);

void biot_addln(const uint8_t *line, struct biot_terminal *term);
void biot_addst(const uint8_t *line, struct biot_terminal *term);
void biot_addch(const uint8_t ch, struct biot_terminal *term);

#endif
