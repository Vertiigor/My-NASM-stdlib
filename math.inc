%macro SUM 3
	mov	rax, %2
	mov	rbx, %3
	add	rax, rbx

	mov	%1, rax
%endmacro

%macro SUB 3
	mov	rax, %2
	mov	rbx, %3
	sub	rax, rbx

	mov	%1, rax
%endmacro


%macro MUL 3
	mov	rax, %2
	mov	rbx, %3
	imul	rbx

	mov	%1, rax
%endmacro

%macro DIV 3
	mov	rax, %2
	cqo
	mov	rbx, %3
	idiv	rbx

	mov	%1, rax
%endmacro

%macro FACTORIAL 2
	mov	rax, %2
	mov	qword [barier], rax
	mov	qword [i], 1

	mov	rax, %2
	cmp	rax, 0
	jle	%%input_error

	%%loop:
		mov	rax, [barier]
		
		cmp	qword [i], rax
		jge	%%done
		
		mov	rax, [result]		
		mov	rbx, [i]
		inc	rbx
		mul	rbx
		
		mov	qword [result], rax

		mov	rcx, [i]
		inc	rcx
		mov	qword [i], rcx

		jmp	%%loop

	%%done:
		xor	rax, rax
		xor	rbx, rbx
		mov	%1, [result]
		jmp	%%end

	%%input_error:
		jmp	%%end

	%%end:
		mov	qword [barier], 1
		mov	qword [result], 1
%endmacro

%macro POWER 3
	mov	rax, %3
	mov	qword [barier], rax
	mov	qword [i], 1

	cmp	rax, 0
	jns	%%loop

	mov	byte [neg_flag], 1
	neg	rax
	mov	qword [barier], rax

	%%loop:
		mov	rax, [barier]
		
		cmp	qword [i], rax
		jg	%%done

		mov	rax, [result]
		mov	rbx, %2
		mul	rbx
		mov	qword [result], rax

		mov	rcx, [i]
		inc	rcx
		mov	qword [i], rcx

		jmp	%%loop

	%%check_sign:
		cmp byte [neg_flag], 1
		jne	%%done
		mov	rax, 1
		sub	rax, [result]
		mov	qword [result], rax

	%%done:
		xor	rax, rax
		xor	rbx, rbx
		mov	%1, [result]
		jmp	%%end

	%%end:
		mov     qword [barier], 1
		mov     qword [result], 1
%endmacro

%macro REMAINDER 3
	mov	rax, %2
	mov	rdx, 0
	div	qword %3
	mov	%1, rdx
%endmacro

section .bss
	i	resq 1
	neg_flag	resb 1

section .data
	result		dq 1
	barier		dq 1
