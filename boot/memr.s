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

memmap_low:
.skip 0x180
memmap_hi:

_mem_detect_hi_e820:
	push	%ebp
	mov	%esp, %ebp

	mov	(memmap_hi), %di
	mov	0x1, (%es):(%di)

	sub	$0x18, %esp
	xor	%ebx, %ebx
	mov	$0xe820, %eax

	pop	%ebp
	ret
	
