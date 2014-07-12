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

# DubOS Boot Sector

# we are in real mode at the moment

.text

.global _start
.code16
_start:
	jmp _preboot
	nop
	## BPB ##

	.ascii	"MSDOS5.0"
	.word	512
	.byte	1
	.word	1
	.byte	2
	.word	224
	.word	2880
	.byte	240
	.word	9
	.word	18
	.word	2
	.long	0
	.long	0

	## EBR ##
	.byte	0x0
	.byte	0x0
	.byte	41
	.long	0x1D107
	.ascii	"DubOS01    "
	.ascii	"FAT12   "


_preboot:
	jmp _boot

	.include "dbs_gdt.inc.s"		# gdt descriptors
	.include "dbs_ibe.inc.s"		# dbs initial boot environment
	.include "dbs_diskop.inc.s"		# dbs disk operator
_boot:
	# ~1.5kb stack
	mov	$0x2000, %sp	# set the stack pointer
	mov	$0x2000, %bp	# set the stack base
	xor	%ax, %ax
	mov	%ax, %ss
	push	%dx		# push the boot drive
	mov	%ax, %ds

	mov	$0xb800, %ax	# vbuf
	mov	%ax, %es	# set the far jump to vbuf
	call	ibe_cls		# clear screen

	mov	$msg_title, %si
	call	ibe_println

	mov	$msg_title2, %si
	call	ibe_println

	call	dbs_geo_flp

	add	$0x2, %sp
	mov	$0xb800, %ax	# vbuf
	mov	%ax, %es
	mov	__flp_buf, %si	# should be loaded
	call	ibe_println

	jmp	.

# Switch to protected mode

msg_title:
	.asciz	"DubOS"
msg_title2:
	.asciz	"Initial Boot Environment"

	.org	.+(510-.), 0x0
	.word	0xAA55

