global find_word

extern string_equals

section .text:
find_word: ;rdi - pointer to string, rsi - pointer to the last node in the dict
	mov		r10, rdi ;r10 string
	mov		r11, rsi ;r11 next_node_address
.loop:	
	mov		rdi, r10
	lea		rsi, [r11 + 8]
	call	string_equals
	jnz		.success
	mov		r11, [r11]
	test	r11b, r11b
	jz		.failure
	jmp		.loop
.success:
	mov		rax, r11
	ret
.failure:
	xor		rax, rax
	ret