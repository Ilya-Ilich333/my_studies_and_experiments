global	_start
%define one 49
%define two 50
%define three 51
%define one_byte_sym  01111111b
%define two_byte_sym  11000000b
%define three_byte_sym  11100000b
%define zero_byte 00000000b

section .data
new_str_sym db 10
number db 0
msg_len equ $-number

section	.text

_start:
;Input the first param in number 
	mov eax, [esp+8]
	mov bl, [eax]
	xor eax, eax 
	mov [number], bl
; Is first param single byte of unicode symbol?
	mov al, [number]
	or eax, one_byte_sym
	cmp eax, one_byte_sym
	je	one_byte
; Is first param first byte of two-byte unicode
; symbol?
two_or_greater_bytes:
	mov al, [number]
	and eax, two_byte_sym
	cmp eax, two_byte_sym
	je	two_bytes
; Is first param first byte of three_byte unicode
; symbol?
three_or_greater_bytes:
	mov al, [number]
	and eax, three_byte_sym
	cmp eax, three_byte_sym
	je	three_bytes
	
one_byte:
	mov byte [number], one
	jmp _exit
two_bytes:
	mov byte [number], two
	jmp _exit
three_bytes:
	mov byte [number], three
	jmp _exit
_exit:
	mov eax, 4
	mov ebx, 1
	mov ecx, number
	mov edx, msg_len
	int 80h
	
	mov eax, 4
	mov ebx, 1
	mov ecx, new_str_sym
	mov edx, 1
	int 80h

	mov eax, 1
	mov ebx, 0
	int 80h
 

