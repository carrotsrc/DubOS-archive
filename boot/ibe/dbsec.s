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
	
	# This is for a FAT12 partition
	.ascii	"MSDOS5.0"
	.word	512		# Bytes/Sector
	.byte	1		# Sectors/Cluster
	.word	1		# Number of reserved sectors
	.byte	2		# number of FAT compies
	.word	224		# number of root dir entries
	.word	2880		# total number of sectors
	.byte	240		# media descriptor type
	.word	9		# sectors/FAT
	.word	18		# sectors/track
	.word	2		# number of heads
	.long	0		# number of hidden sectors
	.long	0		# what?

	## EBR ##
	.byte	0x0
	.byte	0x0
	.byte	41
	.long	0x1D107
	.ascii	"DubOS01    "
	.ascii	"FAT12   "


_preboot:
	jmp _boot

	# ideally we want all this located at 0x0600

	#.include "dbs_gdt.inc.s"		# gdt descriptors - don't need this yet
__RELOCATE_MARKER:
	jmp _post_relocate

	.include "dbs_ibe.inc.s"		# dbs initial boot environment - need this
	.include "dbs_diskop.inc.s"		# dbs disk operator
_boot:
	# ~1.5kb stack
	mov	$0x2000, %sp	# set the stack pointer
	mov	$0x2000, %bp	# set the stack base
	xor	%ax, %ax
	mov	%ax, %ss	# move 0 into stack segment
	push	%dx		# push the boot drive
	mov	%ax, %ds	# mov 0 into data segment
	call	ibe_reloc

_post_relocate:

	mov	$0xb800, %ax	# vbuf
	mov	%ax, %es	# set the far ptr to vbuf
	call	ibe_cls		# clear screen

	mov	$msg_title, %si
	call	ibe_println

	mov	$msg_title2, %si
	call	ibe_println

	call	dbs_lsec

	add	$0x2, %sp
	mov	$0xb800, %ax	# vbuf
	mov	%ax, %es
	mov	__flp_buf, %si	# should be loaded

	call	ibe_println

	jmp	.

msg_title:
	.asciz	"DubOS"
msg_title2:
	.asciz	"Initial Boot Environment"

__RELOCATE_END_MARKER:

	.org	.+(510-.), 0x0
	.word	0xAA55

