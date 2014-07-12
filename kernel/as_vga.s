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

.global _as_vga_enc_char
.global _as_vga_enc_color
_as_vga_enc_char: # (uint8_t ch, uint8_t color)
	xor	%eax, %eax
	movb	0x8(%esp), %ah	# move color to eax[h]
	or	0x4(%esp), %eax # color | ch
	ret			# return ax

_as_vga_enc_color: # (uint8_t fg, uint8_t bg)
	xor	%eax, %eax
	movb	0x8(%esp), %al	# al = bg
	shlb	$0x4, %al	# al << 4

	or	0x4(%esp), %eax	# al | fg
	ret			# return al
