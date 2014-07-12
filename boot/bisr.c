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
#include "kernel/kterm.h"
extern void routine_interrupt_0();
extern void routine_interrupt_stub();

void _dump_reg(uint32_t edi, uint32_t esi, uint32_t ebp, uint32_t esp, 
		uint32_t edx, uint32_t ecx, uint32_t ebx, uint32_t eax)
{
	kprintf("eax: 0x%x  ebx: 0x%x\necx: 0x%x  edx: 0x%x\n", eax, ebx, ecx, edx);
	kprintf("edi: 0x%x  ebp: 0x%x  esp: 0x%x\n", edi, ebp, esp);
	asm("cli");
	asm("hlt");
}

void routine_interrupt_0()
{
	kprint("\n!Exception! [0]\nDivide Fault\n");
	asm("cli");
	asm("hlt");
}

void routine_interrupt_1()
{
	kprint("\n!Exception! [1]\nDebug exception\n");
	asm("cli");
	asm("hlt");
}

void routine_interrupt_2()
{
	kprint("\n!Exception! [2]\nNon maskable interrupt\n");
	asm("cli");
	asm("hlt");
}

void routine_interrupt_3()
{
	kprint("\n!Exception! [3]\nBreakpoint\n");
	asm("cli");
	asm("hlt");
}
void routine_interrupt_4()
{
	kprint("\n!Exception! [4]\nOverflow into instruction\n");
	asm("cli");
	asm("hlt");
}

void routine_interrupt_5()
{
	kprint("\n!Exception! [5]\nBounds check\n");
	asm("cli");
	asm("hlt");
}

void routine_interrupt_6()
{
	kprint("\n!Exception! [6]\nInvalid opcode\n");
	asm("cli");
	asm("hlt");
}

void routine_interrupt_7()
{
	kprint("\n!Exception! [7]\nCo-processor not available\n");
	asm("cli");
	asm("hlt");
}

void routine_interrupt_8()
{
	kprint("\n!Exception! [8]\nDouble fault\n");
	asm("cli");
	asm("hlt");
}

void routine_interrupt_9()
{
	kprint("\n!Exception! [9]\n[Reserved]\n");
	asm("cli");
	asm("hlt");
}

void routine_interrupt_10()
{
	kprint("\n!Exception! [10]\nInvalid TSS\n");
	asm("cli");
	asm("hlt");
}

void routine_interrupt_11()
{
	kprint("\n!Exception! [11]\nSegement not present\n");
	asm("cli");
	asm("hlt");
}

void routine_interrupt_12()
{
	kprint("\n!Exception! [12]\nStack Exception\n");
	asm("cli");
	asm("hlt");
}

void routine_interrupt_13(uint8_t cs, uint8_t error, uint32_t addr)
{
	if(error == 0)
		kprintf("\nGeneral Protection Fault\n@ cs: %d / 0x%x\n", cs, addr);
	else
		kprintf("\nGeneral Protection Fault\ncs: %d Seg: %d 0x%x\n", cs, error, addr);

}

void routine_interrupt_14()
{
	kprint("\n!Exception! [14]\nPage Fault\n");
	asm("cli");
	asm("hlt");
}

void routine_interrupt_15()
{
	kprint("\n!Exception! [15]\nReserved\n");
	asm("cli");
	asm("hlt");
}

void routine_interrupt_16()
{
	kprint("I0x10: Coprocessor Error\n");
	asm("cli");
	asm("hlt");
}

void routine_interrupt_stub()
{
	kprint("Interrupt recieved\n");
	return;
}

