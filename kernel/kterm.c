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
#include "kernel/bio_vbuf.h"
#include "lib/string.h"

#define _KTERM_WIDTH 80
#define _KTERM_HEIGHT 24
#define _KTERM_TABSPACE 4

struct kterm_s {
	size_t row;
	size_t col;

	uint8_t color;
	uint16_t *buffer;
};

struct kterm_s kterm;

void kterm_init()
{
	kterm.row  = 0;
	kterm.col = 0;

	kterm.color = _as_vbuf_enc_color(COLOR_LIGHT_GREY, COLOR_BLACK);
	kterm.buffer = (uint16_t*) _VBUF_ADDR;


	for( size_t y = 0; y < _KTERM_HEIGHT; y++) {
		for( size_t x = 0; x < _KTERM_WIDTH; x++ ) {
			const size_t index  = y * _KTERM_WIDTH + x;

			kterm.buffer[index] = _as_vbuf_enc_char(' ', kterm.color);
		}
	}
}

void kterm_clr()
{
	size_t ts = kterm.row * kterm.col-1;
	while(ts > 0) {
		kterm.buffer[ts] = _as_vbuf_enc_char(' ', kterm.color);
		ts--;
	}

	kterm.buffer[0] = '\0';
}

void kterm_setcolor(uint8_t color)
{
	kterm.color = color;
}

void __kterm_charat(char c, uint8_t color, size_t x, size_t y)
{
	const size_t index = y * _KTERM_WIDTH + x;
	kterm.buffer[index] = _as_vbuf_enc_char(c, color);
}

void __kprint_char (char c)
{
	if(c == '\n') {
		kterm.col = 0;
		kterm.row++;
		return;
	}
	else
	if( c == '\t') {
		uint8_t tab = _KTERM_TABSPACE;
		while(tab-- > 0)
			__kprint_char(' ');

		return;
	}
	

	__kterm_charat(c, kterm.color, kterm.col, kterm.row);
	if( ++kterm.col == _KTERM_WIDTH) {
		kterm.col = 0;
		if( ++kterm.row == _KTERM_HEIGHT) {
			kterm.row = 0;
		}
	}
}

void kprint(const char *data)
{
	size_t len = _strlen(data);
	for( size_t i = 0; i < len; i++)
		__kprint_char(data[i]);
}
