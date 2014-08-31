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

dbs_disk_floppy:
	.byte 0x00

dbs_disk_hdd:
	.byte 0x80

floppy_geo:
	.byte 36, 18, 15, 9, 0

__flp_buf:
	.word 0x7F00		# buffer address

__flp_emsg0:
__flp_emsg1:
	.asciz "Error reading from drive"

dbs_geo_flp:
	
	
	xor	%esi, %esi
	mov	$floppy_geo-1, %si	# load geos

_dbs_g_l:
	push	%bp
	mov	%sp, %bp
	movb	0x6(%bp), %dl

	xorw	%ax, %ax	# int13 00
	int	$0x13		# reset drive


	inc	%si
	movb	$0x2, %cl
	je	_dbs_geo_flp_err1

	xor	%bx, %bx
	movw	%bx, %es
	movw	__flp_buf, %bx	# use the flp buffer
	movw	$0x0201, %ax	# ah = 02h / al = 1 sector to read
	movb	$0x00, %ch	# cylinder 0
	movb	$0x00, %dh	# head 0
	int	$0x13

	jc	_dbs_g_l

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
	call	ibe_print
	pop	%bp
	ret

