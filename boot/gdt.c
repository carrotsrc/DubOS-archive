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
#include "boot/mdesc.h"

extern void gdt_reload();
#define INIT_SEG_NUM 3

/*
 * because the kernel is virtually located at
 * 0xC0100000 we use a base of 0x400000000:
 *
 * 0xC0100000+0x40000000 = 0x100100000
 * 
 * but in 32bit the highest order nibble is 
 * dropped so we have the physical memory location:
 * 0x100000
 */
#define PREPAGE_BASE 0x40000000
struct memtab_desc gdt[INIT_SEG_NUM];
struct memtab_ptr _gp;

void gdt_add_seg(uint16_t num, uint32_t base, uint32_t limit, uint8_t g, uint8_t db,
		uint8_t l, uint8_t avl, uint8_t p, uint8_t dpl, uint8_t s, uint8_t type)
{
	gdt[num].lo = (base&0xFFFF)<<16;	// 0:15 of base address		=> 16:31
	gdt[num].hi = (base>>16)&0xFF;		// 16:23 of base address	=> 0:7
	gdt[num].hi |= ((base>>24)&0xFF)<<24;	// 24:31 of base address	=> 24:31

	gdt[num].lo |= (limit&0xFFFF);		// 0:15 of limit		=> 0:16
	gdt[num].hi |= (((limit>>16)&0xF)<<16);	// 16:19 of limit		=> 16:19

	//	^^[BASE ADDR]			// 8 bits			=> 31:24
	gdt[num].hi |= (g<<23);			// 1 bit			=> 23
	gdt[num].hi |= (db<<22);		// 1 bit			=> 22
	gdt[num].hi |= (l<<21);			// 1 bit			=> 21
	gdt[num].hi |= (avl<<20);		// 1 bit			=> 20
	//	^^[SEG LIMIT]			// 4 bits			=> 19:16
	gdt[num].hi |= (p<<15);			// 1 bit			=> 15
	gdt[num].hi |= (dpl<<13);		// 2 bits			=> 14:13
	gdt[num].hi |= (s<<12);			// 1 bit			=> 12
	gdt[num].hi |= (type<<8);		// 4 bits			=> 11:8
	//	^^[BASE ADDR]			// 8 bits			=> 7:0
}

void gdt_add_null(uint16_t num)
{
	gdt[num].hi = 0x0;
	gdt[num].lo = 0x0;
}

uint8_t gdt_encode_type(uint8_t stype, uint8_t ec, uint8_t wr, uint8_t aa)
{
	uint8_t t = stype<<3;	// 0: Data			1: Code
	t |= ec<<2;		// Expansion Direction		Conforming
	t |= wr<<1;		// Write Enabled		Read Enabled
	t |= aa;		// Accessed			Accessed

	return t;
}

/*
 *  Intel Segment Descriptor
 *
 *  seg desc:
 *  31-24	: base address highest order byte 31:24			8 bit	addr
 *  23		: Granularity 0=1B-1MB 1=4KB-4GB			1 bit	g
 *  22		: Default op size 1=32 0=16				1 bit	db
 *  21		: 64bit code segment 0=32 1=64				1 bit	l
 *  20		: AVL available for use by system software		1 bit	avl
 *  19-16	: Segment limit						4 bit	limit
 *  15		: Present in memory					1 bit	p
 *  14-13	: Priviledge level					2 bit	dpl	0-3
 *  12		: Descriptor type, 0=system 1=code/data			1 bit	s
 *  11-8	: Type							4 bit	type
 *  7-0		: Base address, second higher order byte 23:16		8 bit	addr
 *
 *  31-16	: Base address, lowest order bytes 00:15		16bit	addr
 *  15-0	: Limit, lowest order bytes 00:15			16bit	limit
 *
 *  Type:
 *  11		: 0 =Data segment 		1 =Code segment
 *  10		: E Expansion Down		C Conforming
 *  9		: W Write-Enabled		R Read-Enabled
 *  8		: A Accessed			A Accessed  
 */

 /*
  * num, base, limit, g, db, l, avl, 
  * p, dpl, s, type(stype, ec, wr, aa)
  */
void gdt_init()
{
	_gp.limit = (sizeof(struct memtab_desc)*3)-1;
	_gp.base = (uintptr_t) &gdt;

	// null: +0x00
	gdt_add_null(0);

	// code: +0x08
	gdt_add_seg(1, 0, 0xFFFF, 1, 1, 0, 0, 
		    1, 0, 1, gdt_encode_type(1,0,1,0));

	// data: +0x10
	gdt_add_seg(2, 0, 0xFFFF, 1, 1, 0, 0, 
		    1, 0, 1, gdt_encode_type(0,0,1,0));

	gdt_reload();
}

void prepage_gdt_init()
{
	_gp.limit = (sizeof(struct memtab_desc)*3)-1;
	_gp.base = (uintptr_t) &gdt;

	// null: +0x00
	gdt_add_null(0);

	// code: +0x08
	gdt_add_seg(1, PREPAGE_BASE, 0xFFFF, 1, 1, 0, 0, 
		    1, 0, 1, gdt_encode_type(1,0,1,0));

	// data: +0x10
	gdt_add_seg(2, PREPAGE_BASE, 0xFFFF, 1, 1, 0, 0, 
		    1, 0, 1, gdt_encode_type(0,0,1,0));

	gdt_reload();
}
/*
 * Notes
 * Segment selector: 16bit
 * 	index [bits 3-15]: 1 of 8192 segment index (2^13)
 *	Table Indicator [bit 2]: 1=gdt 0=ldt
 *	Privilege level [bit 0-1]: 0 highest to 3 lowest
 */


/* EXTRA NOTES:
 *
 *  Intel basic descriptor structure
 *  lifted from STARTUP.asm in manual

struct gdt_desc {
	uint16_t limit_0_15;
	uint16_t base_0_15;

	uint8_t base_16_24;
	uint8_t access;
	uint8_t gran;
	uint8_t base_24_31;
} __attribute__((packed));
*/

/*
 * initialize code and data segements
 * lifted from Grub boot.S

struct gdt_desc igdt[3] = {
	{ 0, 0,			// null: +0x00
	0, 0, 0, 0 },

	{ 0xFFFF, 0,		// code: +0x08
	0, 0x9A, 0xCF, 0 },

	{ 0xFFFF, 0,		// data: +0x10
	0, 0x92, 0xCF, 0}
		
};
*/
