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
	
_preboot:
	jmp _boot

	# this is moved to 0x0600 when ibe_reloc is called
	#.include "dbs_gdt.inc.s"		# gdt descriptors - don't need this yet
__RELOCATE_MARKER:
	jmp _post_relocate

	.include "dbs_out.inc.s"		# dbs output - need this
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

	# don't need to call the relocator routine
	# since we're not coming back back here
	jmp	ibe_reloc

_post_relocate:
	mov	$0xb800, %ax	# vbuf
	mov	%ax, %es	# set the far ptr to vbuf
	call	dbs_cls		# clear screen

	call	ibe_load_partition
	jmp	$0x0000, $0x7c00

msg_title:
	.asciz	"DubOS"
msg_title2:
	.asciz	"Initial Boot Environment"

__RELOCATE_END_MARKER:

	# write up to the partition table
	.org	.+(0x1b4-.), 0x0
	.ascii	"[DUBOS-BS]" # disk marker

	/* We're not building a partition table at the moment
	*  so we assume that the boot signature is already
	*  written to the end of the boot sector
	*/
	#.word	0xAA55

