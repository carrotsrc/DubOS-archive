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

dbs_gdt:
	.short dbs_gdt_end - dbs_gdt_gates -1
	.long dbs_gdt_gates

dbs_gdt_gates:
	# 0
	# null descriptor
	.byte 0x00, 0x00	# limit
	.byte 0x00, 0x00	# base lo
	.byte 0x00		# base mid
	.byte 0x00		# access
	.byte 0x00		# granularity
	.byte 0x00		# base hi

	# 8 bytes
	# code descriptor
	.byte 0xFF, 0xFF	# limit
	.byte 0x00, 0x00	# base lo
	.byte 0x00		# base mid
	.byte 0x9a		# access
	.byte 0xcf		# granularity - 32 bit
	#.byte 0x8f		# granularity - 16 bit
	.byte 0x00		# base hi

	# 16 bytes
	# data descriptor
	.byte 0xFF, 0xFF	# limit
	.byte 0x00, 0x00	# base lo
	.byte 0x00		# base mid
	.byte 0x92		# access
	.byte 0xcf		# granularity - 32 bit
	#.byte 0x8f		# granularity - 16 bit
	.byte 0x00		# base hi
dbs_gdt_end:
