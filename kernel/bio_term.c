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
#include "kernel/bio_term.h"

struct biot_terminal biot_init(uint8_t row, uint8_t col, uint8_t clr)
{
	struct biot_terminal term;
	term.row = row;
	term.col = col;
	
	term.cr = 0;
	term.cc = 0;

	term.dflt_clr = clr;
	term.r_off = 0;


	uint16_t buffer[row*col];
	term.buffer = buffer;

	return term;
}

void biot_clr(struct biot_terminal *term)
{
	uint32_t ts = term->row*term->col-1;
	while(ts > 0) {
		term->buffer[ts] = ' ';
		ts--;
	}

	term->buffer[0] = '\0';
}

void scroll_term(struct biot_terminal *term)
{
	uint8_t ch;
	uint8_t i = 0;
	while((ch = term->buffer[i]) != '\n') {
		if(ch == '\0')
			break;

		i++;
	}

	i++;
}

uint32_t get_termloc(struct biot_terminal *term)
{
	uint32_t loc = term->cr * term->col + term->cc;

	if(term->cc == term->col-1) {
		term->cc = 0;
		term->cr++;
		term->buffer[loc] = '\n';
		loc++;
	}

	return loc;
}

void biot_addch(const uint8_t ch, struct biot_terminal *term)
{
	uint32_t loc = get_termloc(term);
	term->buffer[loc] = ch;
	term->buffer[loc+1] = '\0';
	term->cc++;
}
/*
void biot_addln(const uint8_t *line, struct biot_terminal *term)
{
	
}

void biot_addst(const uint8_t *line, struct biot_terminal *term)
{
	
}
*/
