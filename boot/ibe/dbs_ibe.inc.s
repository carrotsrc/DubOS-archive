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

# Load a partition
ibe_load_partition:
	push	%bp
	mov	%sp, %bp
	sub	$0x6, %sp	# enough variables for CHS
	call	ibe_read_ptable
	push	0x4(%bp)	# push the calling disk
	cmp	$0x1, -0x2(%bp)
	jl	ibe_lp_xt
	call	dbs_lsec	# load the boot sector
ibe_lp_xt:
	add	$0x8, %sp # clear out variables
	pop	%bp
	ret

# Read the partition table to find
# bootable partition
ibe_read_ptable:
	push	%bp
	mov	%sp, %bp
	xor	%cx, %cx
	mov	$0x30, %cx	# partition number (ascii)

	xor	%ax, %ax
	xor	%bx, %bx
	mov	$0x7dae, %bx	# first entry - 16 bytes

__ibe_read_pentry:
	add	$0x10, %bx	# next entry
	inc	%cx		# next internal id

	cmp	$0x35, %cl		# if ths is true then we
	je	__ibe_read_ptable_xt	# failed to find any

	mov	%bx, %si
	movb	(%si), %al
	cmp	$0x80, %al
	jne	__ibe_read_pentry

	xor	%bx, %bx
	# found a bootable partition
__ibe_chs_loop:
	inc	%si		# Head
	mov	(%si), %al
	mov	%al, 0x6(%bp)

	inc	%si		# Cylinder
	mov	(%si), %al
	mov	%al, 0x8(%bp)

	inc	%si		# Sector
	mov	(%si), %al
	mov	%al, 0x4(%bp)

__ibe_read_ptable_xt:
	pop	%bp
	ret


__dbs_msg_inactive:
	.asciz ": Inactive"

__dbs_msg_active:
	.asciz ": Active"
