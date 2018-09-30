global exit
global string_length
global print_string
global print_char
global print_newline
global print_uint
global print_int
global read_char
global read_word
global parse_uint
global parse_int
global string_equals
global string_copy
global read_string

section .data
char_10: db 10

section .text
exit:
	mov 	rax, 60
	syscall
	
string_length:
	xor		rax, rax
.loop_strlen:
	cmp 	byte[rdi+rax], 0
	jz 		.end
	inc 	rax
	jmp 	.loop_strlen
.end:
	ret
	
print_string:
	call 	string_length
	mov 	rsi, rdi
	mov		rdx, rax
	mov		rax, 1
	mov		rdi, 1
	syscall
	xor		rax, rax
	ret
	
print_char:
	push	rdi
	mov 	rsi, rsp
	mov		rdx, 1
	mov		rax, 1
	mov		rdi, 1
	syscall
	add		rsp, 8
	xor		rax, rax
	ret
	
print_newline:
	mov 	rsi, char_10
	mov		rdx, 1
	mov		rax, 1
	mov		rdi, 1
	syscall
	xor		rax, rax
	ret
	
print_uint:
	mov 	r10, rsp
	mov 	rax, rdi
	mov 	rcx, 10
	dec		rsp
	mov		byte[rsp], 0
.loop_uint:
	dec		rsp
	xor		rdx, rdx
	div 	rcx
	add		rdx, 0x30
	mov		byte[rsp], dl
	test	rax, rax
	jnz		.loop_uint
	mov		rdi, rsp
	call 	print_string
	mov		rsp, r10
	xor 	rax, rax
	ret
	
print_int:
	mov 	r9, rdi
	cmp		rdi, 0
	jge 	.continue
	mov 	rdi, '-'
	call 	print_char
	neg		r9
.continue:
	mov 	rdi, r9
	call 	print_uint
	ret
	
read_char:
	xor 	rax, rax
	push	rax
	mov 	rax, 0
	mov		rsi, rsp
	mov		rdx, 1
	xor		rax, rax
	xor		rdi, rdi
	syscall
	pop		rax
	ret
	
read_word:
	mov 	r9, rdi
	mov		r8, rsi ;хранит rsi
	xor		r10, r10 ;счетчик
.start_reading:
	call 	read_char
	cmp 	al, 0xA
	jz		.start_reading
	cmp 	al, 0x9
	jz		.start_reading
	cmp 	al, 0x10
	jz		.start_reading
	cmp 	al, 0x20
	jz		.start_reading
	jmp 	.read_word_write
.read_word_loop:
	call 	read_char
	cmp 	al, 0xA
	jz		.end_reading
	cmp 	al, 0x9
	jz		.end_reading
	cmp 	al, 0x10
	jz		.end_reading
	cmp 	al, 0x20
	jz		.end_reading
	cmp 	r8, r10
	je		.read_word_error
.read_word_write:
	test	al, al
	jz		.end_reading
	mov		byte[r9+r10], al
	inc		r10
	jmp 	.read_word_loop
.end_reading:
	mov		byte[r9+r10], 0
	mov		rax, r9
	mov 	rdx, r10
	ret
.read_word_error:
	xor		rax, rax
	ret
	
parse_uint:
	xor		rax, rax ;accumulator ;rdi - arg
	xor		r9, r9 ;counter
	xor		r10, r10 ;counter of nulls
	xor		rdx, rdx
	mov		r8, 10 ;radix
;.nulls:
	;cmp		byte[rdi+r9], 0x30
	;jnz		.loop
	;inc		r9
	;inc 	r10
	;jmp 	.nulls
.parse_uint_loop:
	cmp		byte[rdi+r9], 0x30
	jb		.parse_uint_end
	cmp		byte[rdi+r9], 0x39
	ja		.parse_uint_end
	mul		r8
	sub		byte[rdi+r9], 0x30
	add		al, byte[rdi+r9]
	inc		r9
	jmp 	.parse_uint_loop
.parse_uint_end:
	sub		r9, r10
	mov 	rdx, r9
	ret
	
parse_int:
	cmp 	byte[rdi], 0x2D
	jz		.negative
	call 	parse_uint
	ret
.negative:
	inc		rdi
	call 	parse_uint
	neg		rax
	inc		rdx
	ret
	
string_equals:
	xor		rcx, rcx
	xor 	rax, rax
 .string_equals_loop:
	mov		r8b, byte[rdi+rcx]
	mov		r9b, byte[rsi+rcx]
	inc 	rcx
	cmp		r8b, r9b
	jnz		.not_equals
	cmp		r8b, 0
	jnz		.string_equals_loop
	inc		rax
	ret
.not_equals:
	xor 	rax, rax
	ret

string_copy: ;rdi str rsi getter rdx buffer_size
	xor		rcx, rcx
	cmp		rdx, 0
	jle		.string_copy_error
.string_copy_loop:
	cmp		rcx, rdx
	jz 		.string_copy_end
	mov		r8b, byte[rdi+rcx]
	mov		byte[rsi+rcx], r8b
	inc		rcx
	jmp		.string_copy_loop
.string_copy_end:
	xor		r8, r8
	cmp 	byte[rdi+rcx+1], 0
	jnz		.string_copy_error
	mov		byte[rsi+rcx+1], r8b
	mov		rax, rsi
	ret
.string_copy_error:	
	xor		rax, rax
	ret
	
read_string:
	mov 	r9, rdi
	mov		r8, rsi ;хранит rsi
	xor		r10, r10 ;счетчик
.read_string_loop:
	call 	read_char
	cmp 	al, 0xA
	jz		.end_reading
	test	al, al
	jz		.end_reading
	mov		byte[r9+r10], al
	inc		r10
	cmp 	r8, r10
	jb		.read_string_error
	jmp 	.read_string_loop
.end_reading:
	mov		byte[r9+r10], 0
	mov		rax, r9
	mov 	rdx, r10
	ret
.read_string_error:
	xor		rax, rax
	ret