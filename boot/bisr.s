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

.global isr0
.global isr1
.global isr2
.global isr3
.global isr4
.global isr5
.global isr6
.global isr7
.global isr8
.global isr9
.global isr10
.global isr11
.global isr12
.global isr13
.global isr14
.global isr15
.global isr16

.global isr_stub
#.global isr_test

isr0:
	pusha
	push %gs
	push %fs
	push %ds
	push %es

	.extern routine_interrupt_0
	call routine_interrupt_0

	pop %es
	pop %ds
	pop %fs
	pop %gs
	popa
	iret

isr1:
	pusha
	push %gs
	push %fs
	push %ds
	push %es

	.extern routine_interrupt_1
	call routine_interrupt_1

	pop %es
	pop %ds
	pop %fs
	pop %gs
	popa
	iret
isr2:
	pusha
	push %gs
	push %fs
	push %ds
	push %es

	.extern routine_interrupt_2
	call routine_interrupt_2

	pop %es
	pop %ds
	pop %fs
	pop %gs
	popa
	iret
isr3:
	pusha
	push %gs
	push %fs
	push %ds
	push %es

	.extern routine_interrupt_3
	call routine_interrupt_3

	pop %es
	pop %ds
	pop %fs
	pop %gs
	popa
	iret
isr4:
	pusha
	push %gs
	push %fs
	push %ds
	push %es

	.extern routine_interrupt_4
	call routine_interrupt_4

	pop %es
	pop %ds
	pop %fs
	pop %gs
	popa
	iret
isr5:
	pusha
	push %gs
	push %fs
	push %ds
	push %es

	.extern routine_interrupt_5
	call routine_interrupt_5

	pop %es
	pop %ds
	pop %fs
	pop %gs
	popa
	iret
isr6:
	pusha
	push %gs
	push %fs
	push %ds
	push %es

	.extern routine_interrupt_6
	call routine_interrupt_6

	pop %es
	pop %ds
	pop %fs
	pop %gs
	popa
	iret
isr7:
	pusha
	push %gs
	push %fs
	push %ds
	push %es

	.extern routine_interrupt_7
	call routine_interrupt_7

	pop %es
	pop %ds
	pop %fs
	pop %gs
	popa
	iret
isr8:
	pusha
	push %gs
	push %fs
	push %ds
	push %es

	.extern routine_interrupt_8
	call routine_interrupt_8

	pop %es
	pop %ds
	pop %fs
	pop %gs
	popa
	iret
isr9:
	pusha
	push %gs
	push %fs
	push %ds
	push %es

	.extern routine_interrupt_9
	call routine_interrupt_9

	pop %es
	pop %ds
	pop %fs
	pop %gs
	popa
	iret

isr10:
	pusha
	push %gs
	push %fs
	push %ds
	push %es

	.extern routine_interrupt_10
	call routine_interrupt_10

	pop %es
	pop %ds
	pop %fs
	pop %gs
	popa
	iret

isr11:
	pusha
	push %gs
	push %fs
	push %ds
	push %es

	.extern routine_interrupt_11
	call routine_interrupt_11

	pop %es
	pop %ds
	pop %fs
	pop %gs
	popa
	iret

isr12:
	pusha
	push %gs
	push %fs
	push %ds
	push %es

	.extern routine_interrupt_12
	call routine_interrupt_12

	pop %es
	pop %ds
	pop %fs
	pop %gs
	popa
	iret

isr13:
	pusha
	push 0x20(%esp)	# return address
	push 0x28(%esp)	# error code
	push %cs	# code segement

	.extern routine_interrupt_13
	call routine_interrupt_13

	add	$0xC, %esp

	.extern _dump_reg
	call	_dump_reg

	popa
	iret

isr14:
	pusha
	push %gs
	push %fs
	push %ds
	push %es

	.extern routine_interrupt_14
	call routine_interrupt_14

	pop %es
	pop %ds
	pop %fs
	pop %gs
	popa
	iret

isr15:
	pusha
	push %gs
	push %fs
	push %ds
	push %es

	.extern routine_interrupt_15
	call routine_interrupt_15

	pop %es
	pop %ds
	pop %fs
	pop %gs
	popa
	iret
isr16:
	pusha
	push %gs
	push %fs
	push %ds
	push %es

	.extern routine_interrupt_16
	call routine_interrupt_16

	pop %es
	pop %ds
	pop %fs
	pop %gs
	popa
	iret

isr_stub:
	pusha
	push %gs
	push %fs
	push %ds
	push %es

	.extern routine_interrupt_stub
	call routine_interrupt_stub

	pop %es
	pop %ds
	pop %fs
	pop %gs
	popa
	iret

isr_test:
	int	$0x3
	ret
