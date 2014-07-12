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

# Protected Boot Environment [32bit]
# Boot Sector

# We are working in protected mode now
# clear screen
pbe_cls:
	mov	$0xb8000, %edi	# seg -> paddr is 0xb800 * 0x10 = 0xb8000
	mov	$4000, %ebx
	xor	%esi, %esi
	xor	%eax, %eax

_pbe_clsl:
	stosw
	inc	%esi
	cmp	%ebx, %esi
	jne	_pbe_clsl

	mov	$0x0, %eax
	mov	%eax, __dbs_xpos
	mov	%eax, __dbs_ypos
	ret

# print character array and add cr
pbe_println:
	call	pbe_print
	add	$0x1, __dbs_ypos
	movw	$0x0, %ax
	mov	%ax, __dbs_xpos
	ret

# print character array
pbe_print:
	xor	%eax, %eax
	xor	%ecx, %ecx

_pbe_pr_l:
	mov	(%esi), %eax
	cmp	$0x0, %eax
	je	_pbe_pr_xt
	call	_pbe_pr_u8
	inc	%esi
	jmp 	_pbe_pr_l

_pbe_pr_xt:
	ret

_pbe_pr_u8:
	mov	$0x0f, %ah
	mov	%ax, %cx

	mov	__dbs_ypos, %ax
	mov	$160, %dx
	mul	%dx

	mov	__dbs_xpos, %bx
	shl	$1, %bx

	mov	$0xb8000, %edi	# seg -> paddr is 0xb800 * 0x10 = 0xb8000
	add	%eax, %edi
	add	%ebx, %edi

	mov	%ecx, %eax
	stosw

	add	$1, __dbs_xpos

	ret
