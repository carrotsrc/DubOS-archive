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
#ifndef __MULTIBOOT_H
#define __MUTLIBOOT_H

struct mb_info {
	uint32_t flags;				// 0	-- always

	uint32_t mem_lower;			// 4	-- flag 0
	uint32_t mem_upper;			// 8	-- flag 0

	uint32_t boot_device;			// 12	-- flag 1

	uint32_t cmdline;			// 16	-- flag 2

	uint32_t mods_count;			// 20	-- flag 3
	uintptr_t mods_addr;			// 24	-- flag 3

	uint32_t syms_28;			// 28	|
	uint32_t syms_32;			// 32	|
	uint32_t syms_36;			// 36	|
	uint32_t syms_40;			// 40	-- flag 4 or 5

	uint32_t mmap_length;			// 44	-- flag 6
	uintptr_t mmap_addr;			// 48	-- flag 6

	uint32_t drives_length;			// 52	-- flag 7
	uint32_t drives_addr;			// 56	-- flag 7

	uint32_t config_table;			// 60	-- flag 8

	uint32_t boot_loader_name;		// 64	-- flag 9

	uint32_t apm_table;			// 68	-- flag 10

	uint32_t vbe_control_info;		// 72	|
	uint32_t vbe_mode_info;			// 76	|
	uint16_t vbe_mode;			// 80	|
	uint16_t vbe_interface_seg;		// 82	|
	uint16_t vbe_interface_off;		// 84	|
	uint16_t vbe_interface_len;		// 86	-- flag 11
} __attribute__((packed));

typedef struct mb_memmap {
	uint32_t size;		// 4 bytes
	
	uint32_t base_lo;	// | base addr 8 bytes
	uint32_t base_hi;	// | -----------------

	uint32_t length_lo;	// | length 8 bytes
	uint32_t length_hi;	// | --------------

	uint32_t type;		// type 4 bytes
} mb_memmap_t;
#endif
