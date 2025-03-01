%macro PRINT 2
	mov	rax, sys_write		; System call for write
	mov	rdi, stdout		; Set file descriptor to stdout
	mov	rsi, %1			; Address of data
	mov	rdx, %2			; Length of data
	syscall				; Trigger syscall
%endmacro

%macro PRINT_DECIMAL 1
	mov	rax, %1			; Load number into RAX
	mov	rdi, buffer+31		; Point to end of buffer (null-terminated)
	mov	byte [rdi], 0		; Null terminator
	dec	rdi

	mov	byte [negFlag], 0	; Reset negFlag

	test	rax, rax                       
	jnz	%%check_sign
	mov	byte [rdi], '0'		; Handle zero case
	dec	rdi

	jmp	%%done

	%%check_sign:
		cmp	rax, 0                         
		jns	%%convert	; Skip sign handling if positive

		mov	byte [negFlag], 1	; Set flag to indicate negative number
		neg	rax		; Convert to positive

	%%convert:
		mov	rcx, 10
		mov	rbx, rdi

	%%loop:
		xor	rdx, rdx
		idiv	rcx
		add	dl, '0'
		mov	[rdi], dl
		dec	rdi
		test	rax, rax
		jnz	%%loop

	%%done:
		cmp	byte [negFlag], 1	; Check if the number was negative
		jne	%%skip_sign
		mov	byte [rdi], '-'		; Insert negative sign
		dec	rdi

	%%skip_sign:
		mov	rdx, rbx
		sub	rdx, rdi
		lea	rsi, [rdi+1]

		PRINT	rsi, rdx		; Print number
%endmacro

%macro STRING_TO_DECIMAL 2
	xor	%1, %1			; Clear destination register
	mov	rsi, %2			; Load address of string into RSI
	mov	rdx, 0			; Reset sign flag (0 = positive, 1 = negative)

	movzx	rax, byte [rsi]		; Read first character
	cmp	rax, '-'		; Check if it's a minus sign
	jne	%%loop			; If not, start conversion normally

	mov	rdx, 1			; Set sign flag for negative number
	inc	rsi			; Move past the '-' character

	%%loop:
		movzx	rax, byte [rsi]	; Load next character
		test	rax, rax	; Check for NULL terminator
		jz	%%done		; If NULL, exit loop

		sub	rax, '0'	; Convert ASCII to integer
		cmp	rax, 9		; Validate it's a digit (0-9)
		ja	%%done		; Stop if not a digit

		imul	%1, %1, 10	; Multiply current number by 10
		add	%1, rax		; Add new digit to number

		inc	rsi		; Move to next character
		jmp	%%loop		; Repeat

	%%done:
		test	rdx, rdx	; Check if the number was negative
		jz	%%end		; If not, skip negation
		neg	%1		; Convert to negative

	%%end:
%endmacro


%macro READ 2
	mov	rax, sys_read			; Set system call number for reading input (sys_read)
	mov	rdi, stdin			; Set file descriptor to standard input
	mov	rsi, %1				; Set the memory address where the input will be stored
	mov	rdx, %2				; Specify the maximum number of bytes to read
	syscall					; Trigger the kernel interrupt to execute the system call
%endmacro

%macro NEWLINE 0
	mov	rax, sys_write			; Set system call for write
	mov	rdi, stdout			; Standard output file descriptor
	mov	rsi, newln			; Address of newline character
	mov	rdx, newlnLen			; Length of the newline character
	syscall					; Trigger the kernel interrupt
%endmacro

%macro TERMINATE 0
	mov	rax, 60				; Syscall number for exit
	xor	rdi, rdi			; Exit code 0
	syscall
%endmacro

%macro COPY_STRING_BYTE 3
	cld
	mov	rsi, %2
	mov	rdi, %1
	mov	rcx, %3
	rep	movsb
%endmacro

%macro COPY_STRING_WORD 3
	cld
	mov	rsi, %2
	mov	rdi, %1
	mov	rcx, %3
	rep	movsw
%endmacro

%macro COPY_STRING_DWORD 3
	cld
	mov	rsi, %2
	mov	rdi, %1
	mov	rcx, %3
	rep	movsd
%endmacro

%macro COPY_STRING_QWORD 3
	cld
	mov	rsi, %2
	mov	rdi, %1
	mov	rcx, %3
	rep	movsq
%endmacro

%macro UPPERCASE_BYTE 2
	cld					; Clear direction flag
	mov	rsi, %1				; Source address (string)
	mov	rdi, rsi			; Destination address (same as source, in-place conversion)
	mov	rcx, %2				; String length (no -1)

	%%loop:
		lodsb				; Load a byte from [rsi] into AL
		cmp	al, 'a'			; Check if AL >= 'a'
		jl	%%skip			; If less, skip modification
		cmp	al, 'z'			; Check if AL <= 'z'
		jg	%%skip			; If greater, skip modification
		sub     al, 32			; Convert lowercase to uppercase

	%%skip:
		stosb				; Store modified (or unmodified) byte in [rdi]
		loop    %%loop			; Decrement RCX and loop if not zero
%endmacro

%macro UPPERCASE_WORD 2
	cld					; Clear direction flag
	mov	rsi, %1				; Source address (string)
	mov	rdi, rsi			; Destination address (same as source, in-place conversion)
	mov	rcx, %2				; String length (no -1)
	shr	rcx, 1

	%%loop:
		lodsw				; Load a byte from [rsi] into AL
		cmp	al, 'a'			; Check if AL >= 'a'
		jl	%%skip			; If less, skip modification
		cmp	al, 'z'			; Check if AL <= 'z'
		jg	%%skip			; If greater, skip modification
		sub     al, 32			; Convert lowercase to uppercase

	%%skip:
		stosw				; Store modified (or unmodified) byte in [rdi]
		loop    %%loop			; Decrement RCX and loop if not zero
%endmacro

%macro UPPERCASE_DWORD 2
	cld					; Clear direction flag
	mov	rsi, %1				; Source address (string)
	mov	rdi, rsi			; Destination address (same as source, in-place conversion)
	mov	rcx, %2				; String length (no -1)
	shr	rcx, 2

	%%loop:
		lodsd				; Load a byte from [rsi] into AL
		cmp	al, 'a'			; Check if AL >= 'a'
		jl	%%skip			; If less, skip modification
		cmp	al, 'z'			; Check if AL <= 'z'
		jg	%%skip			; If greater, skip modification
		sub     al, 32			; Convert lowercase to uppercase

	%%skip:
		stosd				; Store modified (or unmodified) byte in [rdi]
		loop    %%loop			; Decrement RCX and loop if not zero
%endmacro

%macro UPPERCASE_QWORD 2
	cld					; Clear direction flag
	mov	rsi, %1				; Source address (string)
	mov	rdi, rsi			; Destination address (same as source, in-place conversion)
	mov	rcx, %2				; String length (no -1)
	shr	rcx, 3

	%%loop:
		lodsq				; Load a byte from [rsi] into AL
		cmp	al, 'a'			; Check if AL >= 'a'
		jl	%%skip			; If less, skip modification
		cmp	al, 'z'			; Check if AL <= 'z'
		jg	%%skip			; If greater, skip modification
		sub     al, 32			; Convert lowercase to uppercase

	%%skip:
		stosq				; Store modified (or unmodified) byte in [rdi]
		loop    %%loop			; Decrement RCX and loop if not zero
%endmacro

%macro LOWERCASE_BYTE 2
	cld					; Clear direction flag
	mov	rsi, %1				; Source address (string)
	mov	rdi, rsi			; Destination address (same as source, in-place conversion)
	mov	rcx, %2				; String length (no -1)

	%%loop:
		lodsb				; Load a byte from [rsi] into AL
		cmp	al, 'A'			; Check if AL >= 'A'
		jl	%%skip			; If less, skip modification
		cmp	al, 'Z'			; Check if AL <= 'Z'
		jg	%%skip			; If greater, skip modification
		add     al, 32			; Convert lowercase to uppercase

	%%skip:
		stosb				; Store modified (or unmodified) byte in [rdi]
		loop    %%loop			; Decrement RCX and loop if not zero
%endmacro

%macro LOWERCASE_WORD 2
	cld					; Clear direction flag
	mov	rsi, %1				; Source address (string)
	mov	rdi, rsi			; Destination address (same as source, in-place conversion)
	mov	rcx, %2				; String length (no -1)
	shr	rcx, 1

	%%loop:
		lodsb				; Load a byte from [rsi] into AL
		cmp	al, 'A'			; Check if AL >= 'A'
		jl	%%skip			; If less, skip modification
		cmp	al, 'Z'			; Check if AL <= 'Z'
		jg	%%skip			; If greater, skip modification
		add     al, 32			; Convert lowercase to uppercase

	%%skip:
		stosb				; Store modified (or unmodified) byte in [rdi]
		loop    %%loop			; Decrement RCX and loop if not zero
%endmacro

%macro LOWERCASE_DWORD 2
	cld					; Clear direction flag
	mov	rsi, %1				; Source address (string)
	mov	rdi, rsi			; Destination address (same as source, in-place conversion)
	mov	rcx, %2				; String length (no -1)
	shr	rcx, 2

	%%loop:
		lodsb				; Load a byte from [rsi] into AL
		cmp	al, 'A'			; Check if AL >= 'A'
		jl	%%skip			; If less, skip modification
		cmp	al, 'Z'			; Check if AL <= 'Z'
		jg	%%skip			; If greater, skip modification
		add     al, 32			; Convert lowercase to uppercase

	%%skip:
		stosb				; Store modified (or unmodified) byte in [rdi]
		loop    %%loop			; Decrement RCX and loop if not zero
%endmacro

%macro LOWERCASE_QWORD 2
	cld					; Clear direction flag
	mov	rsi, %1				; Source address (string)
	mov	rdi, rsi			; Destination address (same as source, in-place conversion)
	mov	rcx, %2				; String length (no -1)
	shr	rcx, 3

	%%loop:
		lodsb				; Load a byte from [rsi] into AL
		cmp	al, 'A'			; Check if AL >= 'A'
		jl	%%skip			; If less, skip modification
		cmp	al, 'Z'			; Check if AL <= 'Z'
		jg	%%skip			; If greater, skip modification
		add     al, 32			; Convert lowercase to uppercase

	%%skip:
		stosb				; Store modified (or unmodified) byte in [rdi]
		loop    %%loop			; Decrement RCX and loop if not zero
%endmacro

%macro ARE_STRINGS_EQUAL 4
	mov	rsi, %1
	mov	rdi, %2
	mov	rcx, %3
	cld

	repe	cmpsb

	je	%%equal

	mov	byte [%4], 0
	mov	al, [%4]
	jmp	%%done

	%%equal:
		mov	byte [%4], 1
		mov	al, [%4]

	%%done:
%endmacro

section .bss
	buffer	resb 32
	negFlag	resb 1

section .data
	kernel		equ 0x80		; Define the kernel interrupt number (0x80) for system calls
	stdout		equ 1			; Define the file descriptor for standard output (stdout)
	sys_write	equ 1			; Define the system call number for writing data (sys_write)
	sys_read	equ 0			; Define the system call number for reading data (sys_read)
	sys_exit	equ 1			; Define the system call number for exiting the program (sys_exit)
	endl		equ 0xa			; Define the newline character (ASCII code 0x0A)
	stdin		equ 0			; Define the file descriptor for standard input (stdin)
	newln		db endl
	newlnLen	equ $ - newln
	minus		db	"-", 0
	minus_len	equ	$ - minus
