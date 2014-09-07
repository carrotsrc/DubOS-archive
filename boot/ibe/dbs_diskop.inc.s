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

__flp_buf:
	.word 0x7C00		# buffer address

__flp_emsg0:
__flp_emsg1:
	.asciz "Error reading from drive"

dbs_lsec:
	push	%bp
	mov	%sp, %bp
	xor	%esi, %esi
	xor	%cx, %cx

dbs_rsdrive:
	# first reset the drive
	movb	0x4(%bp), %dl

	xorw	%ax, %ax	# int13 00
	int	$0x13		# reset drive

	movb	0x6(%bp), %ch	# cylinder
	movb	0x8(%bp), %dh	# head
	movb	0xa(%bp), %cl	# Sector

	xor	%bx, %bx
	movw	%bx, %es
	movw	__flp_buf, %bx	# use the flp buffer

	movw	$0x0201, %ax	# ah = 02h / al = 1 sector to read
	int	$0x13

	jc	dbs_rsdrive	# jump on cf error

	pop	%bp
	ret

# Error message routines
_dbs_geo_flp_err0:
	push	$__flp_emsg0
	call	_dbs_flp_print_msg
	add	$0x2, %esp
	ret

_dbs_geo_flp_err1:
	push	$__flp_emsg1
	call	_dbs_flp_print_msg
	add	$0x2, %esp
	ret

_dbs_flp_print_msg:
	push	%bp
	mov	%sp, %bp
	movw	0x4(%bp), %bx
	movw	%bx, %si
	movw	$0xb800, %ax
	movw	%ax, %es
	call	dbs_print
	pop	%bp
	ret

