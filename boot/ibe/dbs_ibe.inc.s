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

# Initial Boot Environment
# Terminal Inclusion

# clear screen
# assumes vbuf in %ES
ibe_cls:
	mov	$4000, %bx
	xor	%si, %si
	xor	%ax, %ax

_ibe_clsl:
	stosw
	inc	%si
	cmp	%bx, %si
	jne	_ibe_clsl

	ret

# print character array and add cr
# assumes vbuf in %ES
ibe_println:
	call	ibe_print
	add	$0x1, __dbs_ypos
	mov	$0x0, %ax
	mov	%ax, __dbs_xpos
	ret

# print character array
# assumes vbuf in %ES
ibe_print:
	xor	%ax, %ax
	xor	%cx, %cx

_ibe_pr_l:
	mov	(%si), %al
	cmp	$0x0, %al
	je	_ibe_pr_xt
	call	_ibe_pr_u8
	inc	%si
	jmp 	_ibe_pr_l

_ibe_pr_xt:
	ret

_ibe_pr_u8:
	mov	$0x0f, %ah
	mov	%ax, %cx

	mov	__dbs_ypos, %ax
	mov	$160, %dx
	mul	%dx

	mov	__dbs_xpos, %bx
	shl	$1, %bx

	mov	$0x0, %di
	add	%ax, %di
	add	%bx, %di

	mov	%cx, %ax
	stosw

	add	$1, __dbs_xpos

	ret

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

	jmp	$0x0000, $0x0600
__dbs_xpos:
	.word 0

__dbs_ypos:
	.word 0
