# Copyright (C) 2014,  Charlie Fyvie-Gauld (Carrotsrc.org)
#
#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <http://www.gnu.org/licenses/>.

.set ALIGN, 1<<0	# set bit 0- alignment
.set MEMINFO, 1<<1	# set bit 1- memory information
.set FLAGS, ALIGN|MEMINFO
.set MAGIC, 0x1BADB002
.set CHECKSUM, -(MAGIC+FLAGS)
.set MB_HDR_ADDR, 0x101000
.set MB_LDR_ADDR, 0x100000

.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

.section .boot_stack
stack_bottom:
.skip 16384
stack_top:

.section .text

.global start
.global gdt_reload
.global idt_load
.global __pp_gdt
.extern _gp
.extern _idtptr

start:
	lgdt	__pp_gdt
	ljmp	$0x08,$start_hi

start_hi:
	mov	$0x10, %eax		# we reload the segments here
	mov	%eax, %ds
	mov	%eax, %es
	mov	%eax, %fs
	mov	%eax, %gs
	mov	%eax, %ss

	movl	$stack_top, %esp # mov the stack into the stack pointer register (32bit)
	mov	0x44, %eax
	push	%ebx
	call	kmain # run the kernel code

# here we have returned from kernel_main
hang:
	hlt
	jmp	hang

gdt_reload:
	cli
	lgdt	_gp			# reload the gdt pointer
	ljmp 	$0x08,$gdt_rld_seg	# far jump to [codeseg:gdt_rld_seg]

gdt_rld_seg:
	mov	$0x10, %ax		# we reload the segments here
	mov	%eax, %ds
	mov	%eax, %es
	mov	%eax, %fs
	mov	%eax, %gs
	mov	%eax, %ss
	ret

idt_load:
	lidt	_idtptr
	ret

.section .ppinit

__pp_gdt:
	.short __pp_gdt_end - __pp_gdt_gates -1
	.long __pp_gdt_gates

__pp_gdt_gates:
	.byte 0x00, 0x00	# limit
	.byte 0x00, 0x00	# base lo
	.byte 0x00		# base mid
	.byte 0x00		# access
	.byte 0x00		# granularity
	.byte 0x00		# base hi

	.byte 0xFF, 0xFF	# limit
	.byte 0x00, 0x00	# base lo
	.byte 0x00		# base mid
	.byte 0x9a		# access
	.byte 0xcf		# granularity
	.byte 0x40		# base hi

	.byte 0xFF, 0xFF	# limit
	.byte 0x00, 0x00	# base lo
	.byte 0x00		# base mid
	.byte 0x92		# access
	.byte 0xcf		# granularity
	.byte 0x40		# base hi
__pp_gdt_end:
