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

.global uint8_print

__uint8_print:				# num_tochar(num)
	push	%ebp			# push base pointer
	movl	%esp, %ebp		# move stack pointer into base pointer

	mov 	0x8(%esp), %eax		# mov num to eax
	sub	$0x8, %esp		# uint32_t i; uint32_t j;
	
	xor	%ecx, %ecx		# counter  = 0
	
	cmp	$10, %eax		# if (
	jb	ui8p_lpush		# num < 10) goto dec_pops

ui8p_divloop:
	mov	$10, %ebx		# 10 into ebx
	divb	%bl			# divide eax/10
	
	movb	%ah, %dl		# move remainder
	add	$0x30, %edx
	push	%edx			# push remainder
	and	$0xff, %eax		# mask out the remainder
	inc	%cl			# increment counter

	cmp	$10, %eax		# compare new value against 10?
	jae	ui8p_divloop		# eax > 10 then cycle eax again
	
ui8p_lpush:
	add	$0x30, %eax
	push	%eax			# push the final number
	inc	%cl			# increment counter by 1
	mov	%ecx, -0x4(%ebp)	# i = counter
	mov	%ecx, -0x8(%ebp)	# j = counter

ui8p_print:
	call	__kprint_char
	pop	%edx			# pop into edx
	mov	-0x4(%ebp), %ecx	# move i into ecx
	dec	%ecx			# decrement counter
	mov	%ecx, -0x4(%ebp)	# move ecx into i
	cmp	$0, %ecx		# popped everything?
	ja	ui8p_print		# no, go back
	mov	-0x8(%ebp), %eax	# return j;
	add	$0x8, %esp
	pop	%ebp			# pop stack base
	ret				# return

## ---------------------- ##

.global __uint32_print
__uint32_print:
	push	%ebp			# push base pointer
	movl	%esp, %ebp		# move stack pointer into base pointer

	mov 	0x8(%esp), %eax		# mov num to eax
	sub	$0x4, %esp		# uint32_t i; uint32_t j;

	xor	%ecx, %ecx		# counter  = 0

	cmp	$0xa, %eax		# if (
	jb	ui32p_lpush		# num < 10) goto dec_pops

ui32p_divloop:
	xor	%edx, %edx
	mov	$0xa, %ebx		# 10 into ebx
	divl	%ebx			# divide eax/10

	add	$0x30, %edx
	push	%edx			# push remainder
	inc	%ecx			# increment counter

	cmp	$10, %eax		# compare new value against 10?
	jae	ui32p_divloop		# eax > 10 then cycle eax again
	
ui32p_lpush:
	add	$0x30, %eax
	push	%eax			# push the final number
	inc	%ecx			# increment counter by 1
	mov	%ecx, -0x4(%ebp)	# i = counter

ui32p_print:
	call	__kprint_char
	pop	%edx			# pop into edx
	mov	-0x4(%ebp), %ecx	# move i into ecx
	dec	%ecx			# decrement counter
	mov	%ecx, -0x4(%ebp)	# move ecx into i
	cmp	$0, %ecx		# popped everything?
	ja	ui32p_print		# no, go back
	add	$0x4, %esp
	pop	%ebp			# pop stack base
	ret				# return

## ---------------------- ##

.global uint32_charx
__uint32_charx:
	push	%ebp
	movl	%esp, %ebp
	xor	%eax, %eax

	mov	0x8(%ebp), %eax
	add	$0x30, %eax

	cmp	$0x39, %eax
	jbe	ui32_cx_print

	add	$39, %eax

ui32_cx_print:
	push	%eax
	call	__kprint_char
	add	$0x4, %esp

	pop	%ebp
	ret


## ---------------------- ##

.global __uint32_print_x		# 0x18 Param
__uint32_print_x:			# ret addr 0x14
	push	%ebx		# 0x10
	push	%edi		# 0x0C
	push	%esi		# 0x08
	push	%ebp		# 0x04
	mov	%esp, %ebp	# 0x00
	sub	$0x8, %esp	#-0x8

	movl	0x14(%ebp), %eax	# num into eax

	movl	$0x0, -0x4(%ebp)
	cmp	$0x10, %eax
	jb	ui32_px_print_single

	movl	$0x1, %ecx		# set counter to 1
	movl	$0x1, %ebx		# set exponent to 1
	# do this so if it is less than 16 it will go straight to push


# find highest exponent of 16
ui32_px_shl:
	inc	%ecx			# count ++
	shl	$0x4, %ebx		# ebx<<4

	cmp	$0x8, %ecx		# check counter
	je	ui32_px_storec		# we've maxed out the size of a ui32

	cmp	%eax, %ebx
	jbe	ui32_px_shl

	dec	%ecx
	shr	$0x4, %ebx
ui32_px_storec:
	mov	%ecx, -0x4(%ebp)	# i = ecx
	dec	%ecx
	mov	%ecx, -0x8(%ebp)	# j = ecx

# find multiple of current exponent
ui32_px_rsmul:
	cmp	$0x1, %ebx		# if ebx == 1
	je	ui32_px_pop_print_start	# got to pop print (we've reached the end)

	cmp	%ebx, %eax		# if eax < ebx
	jb	ui32_px_push_zero	# push zero (nothing in this exponent)

	xor	%edx, %edx
	xor	%ecx, %ecx

ui32_px_mul:
	add	%ebx, %edx
	inc	%ecx			# ecx++

	# potential fix for 0xffea8000

	# notes
	# -----
	# it's not getting here because when it is on 0xf0000000
	# it is still less than the value so it loops round and
	# screws up because it is overflowing 32bit registers
	cmp	$0xf, %ecx		# are we in danger of overflowing
	je	ui32_px_check_overflow	# yes

	cmp	%eax, %edx
	jb	ui32_px_mul		# if edx < eax, loop


ui32_px_flow:
	# fixes problem with 0xea8000
	cmp	%eax, %edx		# if edx > eax,
	ja	ui32_px_dec_sub		# got to decrement and subtract

	cmp	%eax, %edx		# if edx == eax
	je	ui32_px_on_eq		# push eq



ui32_px_dec_sub:
	dec	%ecx			# ecx --
	sub	%ebx, %edx		# sub ebx to get to the last value

ui32_px_on_eq:
	push	%ecx			# push current multiple
	sub	%edx, %eax		# eax -= edx

	shr	$0x4, %ebx		# ebx >>= 4 (previous exponent of 16)
	jmp	ui32_px_rsmul

ui32_px_check_overflow:

	# are we on an overflow danger zone
	cmp	0x1000000, %ebx
	jb	ui32_px_flow		# nope

	# yes?
	cmp	%eax, %edx
	ja	ui32_px_flow		# it's fine, have multiplied beyond exponent

	# YES! DANGER OF OVERFLOW!
	jmp	ui32_px_on_eq

ui32_px_push_zero:
	push	$0x0
	shr	$0x4, %ebx		# ebx >>=4 (previus exponent of 16)
	jmp	ui32_px_rsmul		# reset and go again


ui32_px_pop_print_start:
	push	%eax

ui32_px_pop_print:
	mov	-0x8(%ebp), %ecx	# count into ecx
	push	(%esp, %ecx, 0x4)	# push esp+(edx*4)
	call	__uint32_charx		# print number
	add	$0x4, %esp		# remove the last push

	mov	-0x8(%ebp), %ecx
	dec	%ecx			# counter --
	mov	%ecx, -0x8(%ebp)

	cmp	$0x0, %ecx
	ja	ui32_px_pop_print

	push	(%esp)
	call	__uint32_charx
	add	$0x4, %esp

	mov	-0x4(%ebp), %eax
	mov	$0x4, %ecx
	mul	%ecx
	add	%eax, %esp

ui32_end_it:
	add	$0x8, %esp
	pop	%ebp
	pop	%esi
	pop	%edi
	pop	%ebx
	ret

ui32_px_print_single:
	push	%eax
	call	__uint32_charx
	add	$0x4, %esp
	jmp	ui32_end_it

skydrop:
	push	$0x41
	call	__kprint_char
	cli
	hlt
