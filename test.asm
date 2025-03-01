%include "stdlib.inc"
%include "math.inc"

section .bss
	input	resq 64

section .data
	message			db	"Type any positive number: ", 0
	message_length		equ	$ - message

	sum_message		db	" + ", 0
	sum_message_len		equ	$ - sum_message

	sub_message		db	" - ", 0
	sub_message_len		equ	$ - sub_message

	mul_message		db	" * ", 0
	mul_message_len		equ	$ - mul_message

	div_message		db	" / ", 0
	div_message_len		equ	$ - div_message

	factorial_message	db	"!", 0
	factorial_message_len	equ	$ - factorial_message

	equal_message		db	" = ", 0
	equal_message_len	equ	$ - equal_message

	power_message		db	"^", 0
	power_message_len	equ	$ - power_message

	remainder_message	db	" % ", 0
	remainder_message_len	equ	$ - remainder_message

	first			dq	0
	second			dq	0
	sum_v			dq	0
	sub_v			dq	0
	mul_v			dq	0
	div_v			dq	0
	power			dq	0
	factorial		dq	0
	remainder		dq	0

section .text
	global	_start

_start:
	PRINT			message, message_length
	READ			input, 64
	STRING_TO_DECIMAL	rbx, input
	mov			[first], rbx

	PRINT			message, message_length
	READ			input, 64
	STRING_TO_DECIMAL	rcx, input
	mov			[second], rcx

	NEWLINE

	SUM			rbx, [first], [second]
	mov			[sum_v], rbx

	SUB			rbx, [first], [second]
	mov			[sub_v], rbx

	MUL			rbx, [first], [second]
	mov			[mul_v], rbx

	DIV			rbx, [first], [second]
	mov			[div_v], rbx

	FACTORIAL		rbx, [first]
	mov			[factorial], rbx

	POWER			rbx, [first], [second]
	mov			[power], rbx

	REMAINDER		rbx, [first], [second]
	mov			[remainder], rbx

	PRINT_DECIMAL		[first]
	PRINT			sum_message, sum_message_len
	PRINT_DECIMAL		[second]
	PRINT			equal_message, equal_message_len
	PRINT_DECIMAL		[sum_v]
	
	NEWLINE

	PRINT_DECIMAL		[first]
	PRINT			sub_message, sub_message_len
	PRINT_DECIMAL		[second]
	PRINT			equal_message, equal_message_len
	PRINT_DECIMAL		[sub_v]

	NEWLINE

	PRINT_DECIMAL		[first]
	PRINT			mul_message, mul_message_len
	PRINT_DECIMAL		[second]
	PRINT			equal_message, equal_message_len
	PRINT_DECIMAL		[mul_v]

	NEWLINE

	PRINT_DECIMAL		[first]
	PRINT			div_message, div_message_len
	PRINT_DECIMAL		[second]
	PRINT			equal_message, equal_message_len
	PRINT_DECIMAL		[div_v]

	NEWLINE

	PRINT_DECIMAL 		[first]
	PRINT			factorial_message, factorial_message_len
	PRINT			equal_message, equal_message_len
	PRINT_DECIMAL		[factorial]

	NEWLINE

	PRINT_DECIMAL		[first]
	PRINT			power_message, power_message_len
	PRINT_DECIMAL		[second]
	PRINT			equal_message, equal_message_len
	PRINT_DECIMAL		[power]

	NEWLINE

	PRINT_DECIMAL		[first]
	PRINT			remainder_message, remainder_message_len
	PRINT_DECIMAL		[second]
	PRINT			equal_message, equal_message_len
	PRINT_DECIMAL		[remainder]

	NEWLINE	

	TERMINATE
