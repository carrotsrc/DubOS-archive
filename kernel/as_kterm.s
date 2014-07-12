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

.global kprintf

kprintf: # (const char *str, ...)
	push	%edi
	push	%esi
	push	%ebx

	push	%ebp
	mov	%esp, %ebp

	sub	$0x8, %esp		# uint8_t i, uint8_t arg

	movl	$0x0, -0x4(%ebp)	# i = 0
	movl	$0x0, -0x8(%ebp)	# arg = 0

_kprintf_loop:
	xor	%edx, %edx
	mov	0x14(%ebp), %eax	# eax = str
	movl	-0x4(%ebp), %ecx	# ecx = i

	movb	(%eax, %ecx), %dl	# edx = str[i]
	cmp	$0x0, %dl		# if str[i] == 0
	je	_kprintf_end		# goto fin

	
	cmp	$0x25, %dl		# if str[i] == '%'
	je	_kprintf_fmt


	inc	%ecx			# i++
	movl	%ecx, -0x4(%ebp)	# i = ecx

	push	%edx			# 
	call	__kprint_char		# kprint_char str+ecx
	add	$0x4, %esp		# pop back
	jmp	_kprintf_loop

_kprintf_end:
	add	$0x8, %esp

	pop	%ebp

	pop	%ebx
	pop	%esi
	pop	%edi

	ret

_kprintf_fmt:
	inc	%ecx			# i++
	movb	(%eax, %ecx), %dl	# str[i]
	inc	%ecx			# i++
	movl	%ecx, -0x4(%ebp)	# store i

	cmp	$0x64, %dl
	je	_kprintf__d

	cmp	$0x78, %dl
	je	_kprintf__x

	jmp	_kprintf_loop		# we've processed the value

_kprintf__d:
	movl	-0x8(%ebp), %ecx		# ecx = arg
	movl	0x18(%ebp, %ecx, 4), %edx	# mov (ebp+24) + ecx*4

	inc	%ecx				# ecx++
	movl	%ecx, -0x8(%ebp)		# store ecx in arg
	
	push 	%edx
	call	__uint32_print
	add	$0x4, %esp			# move stack up

	jmp	_kprintf_loop

_kprintf__x:
	movl	-0x8(%ebp), %ecx		# ecx = arg
	movl	0x18(%ebp, %ecx, 4), %edx	# mov (ebp+24) + ecx*4

	inc	%ecx				# ecx++
	movl	%ecx, -0x8(%ebp)		# store ecx in arg
	
	push 	%edx
	call	__uint32_print_x
	add	$0x4, %esp			# move stack up

	jmp	_kprintf_loop
