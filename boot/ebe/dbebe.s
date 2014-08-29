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

.code16
ebemsg:
	.asciz "Extended Boot Environment"

#dbs_sw_pmode:
#.code16
#	xorw	%ax, %ax
#	movw	%ax, %ds
#	cli

#	DATA32 ADDR32	lgdt	dbs_gdt

#	mov	%cr0, %eax
#	or	$1, %al
#	mov	%eax, %cr0

#	DATA32	ljmp	$0x08, $_start_pmode

# Here we reload all the segements with
# the descriptors

#_start_pmode:
#.code32
#	movw	$0x10, %ax	# set as data descriptor
#	movw	%ax, %ds
#	movw	%ax, %es
#	movw	%ax, %fs
#	movw	%ax, %gs
#	movw	%ax, %ss

#	mov	$msg_pmtitle, %esi
	#call	pbe_print
#	jmp	.

