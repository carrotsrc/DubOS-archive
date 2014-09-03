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

# we need to relocate the boot code for chainloading
ibe_reloc:
	mov	$0x060, %ax
	mov	%ax, %es	# set far ptr to 0x0600
	mov	$__RELOCATE_MARKER, %si
	xor	%ax, %ax
	mov	%ax, %di

_ibe_reloc_l:
	mov	(%si), %al
	stosb
	inc	%si
	cmp	$__RELOCATE_END_MARKER, %si
	jl	_ibe_reloc_l

	xor	%di, %di
	jmp	$0x0000, $0x0600 # far jump to new location

ibe_read_ptable:
	xor	%ax, %ax
	xor	%bx, %bx
	mov	$0x7dbe, %bx
	mov	%bx, %si
	movb	(%si), %al
	cmp	$0x80, %al
	je	__ibe_read_active
	ret

__ibe_read_active:
	mov	$0xb800, %ax
	mov	%ax, %es
	mov	$__dbs_msg_active, %si
	call	dbs_println
	ret

__dbs_xpos:
	.word 0

__dbs_ypos:
	.word 0

__dbs_msg_active:
	.asciz "Active"
