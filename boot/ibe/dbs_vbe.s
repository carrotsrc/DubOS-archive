# Copyright (C) 2014, Charlie Fyvie-Gauld (Carrotsrc.org)
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.


# DubOS Volume Boot Sector
# we are in real mode at the moment
.text
.global _start
.code16
_start:
jmp _onload
nop
## BPB ##
# This is for a FAT16 partition
.ascii	"MSDOS5.0"
.word	512		# Bytes/Sector
.byte	1		# Sectors/Cluster
.word	1		# Number of reserved sectors
.byte	2		# number of FAT copies
.word	512		# number of root dir entries
.word	61340		# total number of sectors
.byte	0xf8		# media descriptor type
.word	9		# sectors/FAT
.word	63		# sectors/track
.word	16		# number of heads
.long	0		# number of hidden sectors
.long	0		# number of large sectors
## EBR ##
.byte	0x80		# Physical driver number
.byte	0x00		# Reserved
.byte	0x29		# Extended boot signature
.long	0xD0BD0B	# Volume serial
.ascii	"DubOS01 "	# Volume label
.ascii	"FAT16 "	# File system type

_onload:
	jmp	.

.org	.+(510-.), 0x0
.word	0xAA55
