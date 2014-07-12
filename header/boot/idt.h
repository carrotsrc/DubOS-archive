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
#ifndef __IDT_H
#define __IDT_H

enum intex_code {
	DE = 0,
	DB = 1,
	NMI = 2,
	BP = 3,
	OF = 4,
	BR = 5,
	UD = 6,
	NM = 7,
	DF = 8,
	CSO = 9,
	TS = 10,
	NP = 11,
	SS = 12,
	GP = 13,
	PF = 14,
	MF = 15,
	AC = 16,
	MC = 17,
	XM = 18
};


void idt_init();
void pic_init(uint8_t pic1, uint8_t pic2);

#endif
