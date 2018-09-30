global _start

extern exit
extern find_word
extern read_string
extern print_string

%define MAX_BUFFER_LENGTH 255

section .data
%include "words.inc"
string: times MAX_BUFFER_LENGTH+1 db 0
buffer_overflow_error_message: db "Buffer overflow error!", 0
not_found_error_message_start: db 'String "', 0
not_found_error_message_end: db '" not found!', 0
success_message_start: db 'String "', 0
success_message_end: db '" was successfully founded!', 0

section .text
_start:
	mov		rdi, string
	mov		rsi, MAX_BUFFER_LENGTH
	call	read_string
	test	rax, rax
	jz		.overflow_error

	mov		rdi, string
	mov		rsi, first_word
	call	find_word
	test	rax, rax
	jz		.not_found_error
	mov		rdi, success_message_start
	call	print_string
	mov		rdi, string
	call	print_string
	mov		rdi, success_message_end
	call	print_string
	xor		rdi, rdi
	call	exit
	
.overflow_error:
	mov		rdi, buffer_overflow_error_message
	call	print_string
	mov		rdi, 1
	call	exit
	
.not_found_error:
	mov		rdi, not_found_error_message_start
	call	print_string
	mov		rdi, string
	call	print_string
	mov		rdi, not_found_error_message_end
	call	print_string
	mov		rdi, 1
	call	exit