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
#include "io/port_io.h"
#include "boot/mdesc.h"
#define IDT_SIZE 256

#define PIC1 0x20
#define PIC1_DATA 0x21
#define PIC2 0xA0
#define PIC2_DATA 0xA1

#define ICW1 0x11
#define ICW4 0x01

extern void idt_load();
extern void isr_stub();
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();

struct memtab_desc idt[IDT_SIZE] __attribute__((aligned (8)));
struct memtab_ptr _idtptr;

/*
 *  Intel Description of Interrupt Gate
 *  Lo
 *  00-15	: offset lowest order bytes	offset 00:15
 *  16-31	: Segement Selector
 *
 *  Hi
 *  00-03	: reserved
 *  05-07	: h) 0 0 0 (l
 *  08-10	: h) 1 1 0 (l
 *  11		: size of gate 1=32 0=16	d
 *  12		: 0
 *  13-14	: priviledge level		dpl
 *  15		: present			p
 *  16-31	: offset highest order bytes	offset 16:31
 */
void idt_add_intr(uint8_t num, uintptr_t offset, uint16_t segsel, 
			uint8_t d, uint8_t dpl, uint8_t p)
{
	idt[num].lo = segsel;
	idt[num].lo <<= 16;				// 16:31
	idt[num].lo |= (offset&0xFFFF);			// 00:15
	
	idt[num].hi = ((offset>>16)&0xFFFF)<<16;	// 16:31
	idt[num].hi |= p<<15;				// 15
	idt[num].hi |= dpl<<13;				// 14:13
	idt[num].hi |= (0x60|(d<<7))<<4;		// 04:11
	//	[RESERVED]				// 00:03
	
}

void idt_init()
{
	_idtptr.limit = sizeof(idt)-1;
	_idtptr.base = (uintptr_t)&idt;
	
	idt_add_intr(0,   (uintptr_t)&isr0, 0x08, 1, 0, 1);
	idt_add_intr(1,   (uintptr_t)&isr1, 0x08, 1, 0, 1);
	idt_add_intr(2,   (uintptr_t)&isr2, 0x08, 1, 0, 1);
	idt_add_intr(3,   (uintptr_t)&isr3, 0x08, 1, 0, 1);
	idt_add_intr(4,   (uintptr_t)&isr4, 0x08, 1, 0, 1);
	idt_add_intr(5,   (uintptr_t)&isr5, 0x08, 1, 0, 1);
	idt_add_intr(6,   (uintptr_t)&isr6, 0x08, 1, 0, 1);
	idt_add_intr(7,   (uintptr_t)&isr7, 0x08, 1, 0, 1);
	idt_add_intr(8,   (uintptr_t)&isr8, 0x08, 1, 0, 1);
	idt_add_intr(9,   (uintptr_t)&isr9, 0x08, 1, 0, 1);
	idt_add_intr(10,  (uintptr_t)&isr10, 0x08, 1, 0, 1);
	idt_add_intr(11,  (uintptr_t)&isr11, 0x08, 1, 0, 1);
	idt_add_intr(12,  (uintptr_t)&isr12, 0x08, 1, 0, 1);
	idt_add_intr(13,  (uintptr_t)&isr13, 0x08, 1, 0, 1);
	idt_add_intr(14,  (uintptr_t)&isr14, 0x08, 1, 0, 1);
	idt_add_intr(15,  (uintptr_t)&isr15, 0x08, 1, 0, 1);
	idt_add_intr(16,  (uintptr_t)&isr16, 0x08, 1, 0, 1);
	idt_add_intr(17,  (uintptr_t)&isr_stub, 0x08, 1, 0, 1);
	idt_add_intr(18,  (uintptr_t)&isr_stub, 0x08, 1, 0, 1);
	idt_add_intr(19,  (uintptr_t)&isr_stub, 0x08, 1, 0, 1);
	idt_add_intr(20,  (uintptr_t)&isr_stub, 0x08, 1, 0, 1);
	idt_add_intr(21,  (uintptr_t)&isr_stub, 0x08, 1, 0, 1);
	idt_add_intr(22,  (uintptr_t)&isr_stub, 0x08, 1, 0, 1);
	idt_add_intr(23,  (uintptr_t)&isr_stub, 0x08, 1, 0, 1);
	idt_add_intr(24,  (uintptr_t)&isr_stub, 0x08, 1, 0, 1);
	idt_add_intr(25,  (uintptr_t)&isr_stub, 0x08, 1, 0, 1);
	idt_add_intr(26,  (uintptr_t)&isr_stub, 0x08, 1, 0, 1);
	idt_add_intr(27,  (uintptr_t)&isr_stub, 0x08, 1, 0, 1);
	idt_add_intr(28,  (uintptr_t)&isr_stub, 0x08, 1, 0, 1);
	idt_add_intr(29,  (uintptr_t)&isr_stub, 0x08, 1, 0, 1);
	idt_add_intr(30,  (uintptr_t)&isr_stub, 0x08, 1, 0, 1);
	idt_add_intr(31,  (uintptr_t)&isr_stub, 0x08, 1, 0, 1);

	idt_load();
}

void pic_init(uint8_t pic1, uint8_t pic2)
{
	outb(PIC1, ICW1);
	outb(PIC2, ICW1);

	outb(PIC1+1, pic1);
	outb(PIC2+1, pic2);

	outb(PIC1+1, 4);
	outb(PIC2+1, 2);

	outb(PIC1+1, ICW4);
	outb(PIC2+1, ICW4);

	outb(PIC1+1, 0xFF);
}
