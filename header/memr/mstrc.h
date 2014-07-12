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
#ifndef __MSTRC_H
#define __MSTRC_H

struct _mmap_entry {
	uint32_t size;		// 4 bytes
	
	uint32_t base_lo;	// | base addr 8 bytes
	uint32_t base_hi;	// | -----------------

	uint32_t length_lo;	// | length 8 bytes
	uint32_t length_hi;	// | --------------

	uint32_t type;		// type 4 bytes
};

#endif
