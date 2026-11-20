	.file	"RegisterFileBenchmark.cpp"
	.intel_syntax noprefix
	.text
	.section .rdata,"dr"
	.align 8
.LC1:
	.ascii "C:/msys64/ucrt64/include/benchmark/state.h\0"
.LC2:
	.ascii "cached_ > 0\0"
	.data
	.align 32
.LC0:
	.quad	4294967295
	.quad	0
	.quad	0
	.quad	0
	.quad	4294967295
	.quad	0
	.quad	0
	.quad	0
	.quad	4294967295
	.quad	0
	.quad	0
	.quad	0
	.quad	4294967295
	.quad	0
	.quad	0
	.quad	0
	.quad	4294967295
	.quad	0
	.quad	0
	.quad	0
	.quad	4294967295
	.quad	0
	.quad	0
	.quad	0
	.quad	4294967295
	.quad	0
	.quad	0
	.quad	0
	.quad	4294967295
	.quad	0
	.quad	0
	.quad	0
	.quad	-1
	.quad	0
	.quad	0
	.quad	0
	.quad	-1
	.quad	0
	.quad	0
	.quad	0
	.quad	-1
	.quad	0
	.quad	0
	.quad	0
	.quad	-1
	.quad	0
	.quad	0
	.quad	0
	.quad	-1
	.quad	0
	.quad	0
	.quad	0
	.quad	-1
	.quad	0
	.quad	0
	.quad	0
	.quad	-1
	.quad	0
	.quad	0
	.quad	0
	.quad	-1
	.quad	0
	.quad	0
	.quad	0
	.text
	.p2align 4
	.def	"_ZN12_GLOBAL__N_1L25BM_RegisterFile_EnumWriteERN9benchmark5StateE";	.scl	3;	.type	32;	.endef
	.seh_proc	"_ZN12_GLOBAL__N_1L25BM_RegisterFile_EnumWriteERN9benchmark5StateE"
"_ZN12_GLOBAL__N_1L25BM_RegisterFile_EnumWriteERN9benchmark5StateE":
.LFB7855:
	push	r13
	.seh_pushreg	r13
	push	r12
	.seh_pushreg	r12
	push	rbp
	.seh_pushreg	rbp
	push	rdi
	.seh_pushreg	rdi
	push	rsi
	.seh_pushreg	rsi
	push	rbx
	.seh_pushreg	rbx
	sub	rsp, 552
	.seh_stackalloc	552
	.seh_endprologue
	mov	r8d, 512
	lea	rdx, .LC0[rip]
	mov	rbx, rcx
	lea	rcx, 32[rsp]
	call	"memcpy"
	mov	eax, DWORD PTR 28[rbx]
	test	eax, eax
	je	.L2
	mov	rcx, rbx
	call	[QWORD PTR __imp__ZN9benchmark5State16StartKeepRunningEv[rip]]
.L3:
	mov	rcx, rbx
	call	[QWORD PTR __imp__ZN9benchmark5State17FinishKeepRunningEv[rip]]
	nop
	add	rsp, 552
	pop	rbx
	pop	rsi
	pop	rdi
	pop	rbp
	pop	r12
	pop	r13
	ret
	.p2align 4,,10
	.p2align 3
.L2:
	mov	rsi, QWORD PTR 16[rbx]
	mov	rcx, rbx
	call	[QWORD PTR __imp__ZN9benchmark5State16StartKeepRunningEv[rip]]
	test	rsi, rsi
	je	.L3
	mov	edi, 0
	mov	eax, 0
	mov	edx, 0
	jle	.L14
	.p2align 4
	.p2align 3
.L6:
	mov	r8, rax
	add	rax, 1
	mov	r9, rdx
	adc	rdx, 0
	and	r8, QWORD PTR 64[rsp]
	and	r9, QWORD PTR 72[rsp]
	mov	QWORD PTR 80[rsp], r8
	mov	QWORD PTR 88[rsp], r9
	mov	rcx, rax
	mov	r8, rdx
	xor	rcx, rsi
	xor	r8, rdi
	or	rcx, r8
	jne	.L6
	jmp	.L3
	.p2align 4,,10
	.p2align 3
.L14:
	mov	rbp, QWORD PTR __imp__assert[rip]
	xor	r12d, r12d
	xor	r13d, r13d
.L5:
	mov	rax, r12
	add	r12, 1
	mov	rdx, r13
	adc	r13, 0
	and	rax, QWORD PTR 64[rsp]
	and	rdx, QWORD PTR 72[rsp]
	mov	QWORD PTR 80[rsp], rax
	mov	QWORD PTR 88[rsp], rdx
	lea	rdx, .LC1[rip]
	mov	r8d, 257
	lea	rcx, .LC2[rip]
	call	rbp
	mov	rax, r12
	mov	rdx, r13
	xor	rax, rsi
	xor	rdx, rdi
	or	rax, rdx
	jne	.L5
	jmp	.L3
	.seh_endproc
	.p2align 4
	.def	"_ZN12_GLOBAL__N_1L16BM_Register_CopyERN9benchmark5StateE";	.scl	3;	.type	32;	.endef
	.seh_proc	"_ZN12_GLOBAL__N_1L16BM_Register_CopyERN9benchmark5StateE"
"_ZN12_GLOBAL__N_1L16BM_Register_CopyERN9benchmark5StateE":
.LFB7854:
	push	r13
	.seh_pushreg	r13
	push	r12
	.seh_pushreg	r12
	push	rdi
	.seh_pushreg	rdi
	push	rsi
	.seh_pushreg	rsi
	push	rbx
	.seh_pushreg	rbx
	sub	rsp, 64
	.seh_stackalloc	64
	.seh_endprologue
	mov	eax, 4294967295
	xor	edx, edx
	pxor	xmm0, xmm0
	mov	QWORD PTR 32[rsp], rax
	mov	eax, DWORD PTR 28[rcx]
	mov	rsi, rcx
	mov	QWORD PTR 40[rsp], rdx
	movups	XMMWORD PTR 48[rsp], xmm0
	test	eax, eax
	je	.L16
	call	[QWORD PTR __imp__ZN9benchmark5State16StartKeepRunningEv[rip]]
.L17:
	mov	rcx, rsi
	call	[QWORD PTR __imp__ZN9benchmark5State17FinishKeepRunningEv[rip]]
	nop
	add	rsp, 64
	pop	rbx
	pop	rsi
	pop	rdi
	pop	r12
	pop	r13
	ret
	.p2align 4,,10
	.p2align 3
.L16:
	mov	rdi, QWORD PTR 16[rcx]
	call	[QWORD PTR __imp__ZN9benchmark5State16StartKeepRunningEv[rip]]
	test	rdi, rdi
	je	.L17
	movabs	r8, 1311768467463790320
	mov	r9d, 0
	jle	.L26
	.p2align 4
	.p2align 3
.L18:
	mov	rcx, r8
	mov	rbx, r9
	and	rcx, QWORD PTR 32[rsp]
	and	rbx, QWORD PTR 40[rsp]
	mov	QWORD PTR 48[rsp], rcx
	mov	QWORD PTR 56[rsp], rbx
	sub	rdi, 1
	jne	.L18
	jmp	.L17
	.p2align 4,,10
	.p2align 3
.L26:
	mov	rbx, rdi
	mov	rdi, QWORD PTR __imp__assert[rip]
	mov	r12, r8
	mov	r13, r9
.L19:
	mov	rax, r12
	mov	rdx, r13
	and	rax, QWORD PTR 32[rsp]
	and	rdx, QWORD PTR 40[rsp]
	mov	QWORD PTR 48[rsp], rax
	mov	QWORD PTR 56[rsp], rdx
	mov	r8d, 257
	lea	rdx, .LC1[rip]
	lea	rcx, .LC2[rip]
	call	rdi
	sub	rbx, 1
	jne	.L19
	jmp	.L17
	.seh_endproc
	.p2align 4
	.def	"_ZN12_GLOBAL__N_1L17BM_Register_WriteERN9benchmark5StateE";	.scl	3;	.type	32;	.endef
	.seh_proc	"_ZN12_GLOBAL__N_1L17BM_Register_WriteERN9benchmark5StateE"
"_ZN12_GLOBAL__N_1L17BM_Register_WriteERN9benchmark5StateE":
.LFB7849:
	push	r13
	.seh_pushreg	r13
	push	r12
	.seh_pushreg	r12
	push	rbp
	.seh_pushreg	rbp
	push	rdi
	.seh_pushreg	rdi
	push	rsi
	.seh_pushreg	rsi
	push	rbx
	.seh_pushreg	rbx
	sub	rsp, 72
	.seh_stackalloc	72
	.seh_endprologue
	mov	eax, 4294967295
	xor	edx, edx
	pxor	xmm0, xmm0
	mov	QWORD PTR 32[rsp], rax
	mov	eax, DWORD PTR 28[rcx]
	mov	rbx, rcx
	mov	QWORD PTR 40[rsp], rdx
	movups	XMMWORD PTR 48[rsp], xmm0
	test	eax, eax
	je	.L28
	call	[QWORD PTR __imp__ZN9benchmark5State16StartKeepRunningEv[rip]]
.L29:
	mov	rcx, rbx
	call	[QWORD PTR __imp__ZN9benchmark5State17FinishKeepRunningEv[rip]]
	nop
	add	rsp, 72
	pop	rbx
	pop	rsi
	pop	rdi
	pop	rbp
	pop	r12
	pop	r13
	ret
	.p2align 4,,10
	.p2align 3
.L28:
	mov	rsi, QWORD PTR 16[rcx]
	call	[QWORD PTR __imp__ZN9benchmark5State16StartKeepRunningEv[rip]]
	test	rsi, rsi
	je	.L29
	mov	edi, 0
	mov	eax, 0
	mov	edx, 0
	jle	.L39
	.p2align 4
	.p2align 3
.L32:
	mov	r8, rax
	add	rax, 1
	mov	r9, rdx
	adc	rdx, 0
	and	r8, QWORD PTR 32[rsp]
	and	r9, QWORD PTR 40[rsp]
	mov	QWORD PTR 48[rsp], r8
	mov	QWORD PTR 56[rsp], r9
	mov	rcx, rax
	mov	r8, rdx
	xor	rcx, rsi
	xor	r8, rdi
	or	rcx, r8
	jne	.L32
	jmp	.L29
	.p2align 4,,10
	.p2align 3
.L39:
	mov	rbp, QWORD PTR __imp__assert[rip]
	xor	r12d, r12d
	xor	r13d, r13d
.L31:
	mov	rax, r12
	add	r12, 1
	mov	rdx, r13
	adc	r13, 0
	and	rax, QWORD PTR 32[rsp]
	and	rdx, QWORD PTR 40[rsp]
	mov	QWORD PTR 48[rsp], rax
	mov	QWORD PTR 56[rsp], rdx
	lea	rdx, .LC1[rip]
	mov	r8d, 257
	lea	rcx, .LC2[rip]
	call	rbp
	mov	rax, r12
	mov	rdx, r13
	xor	rax, rsi
	xor	rdx, rdi
	or	rax, rdx
	jne	.L31
	jmp	.L29
	.seh_endproc
	.p2align 4
	.def	"_ZN12_GLOBAL__N_1L16BM_Register_ReadERN9benchmark5StateE";	.scl	3;	.type	32;	.endef
	.seh_proc	"_ZN12_GLOBAL__N_1L16BM_Register_ReadERN9benchmark5StateE"
"_ZN12_GLOBAL__N_1L16BM_Register_ReadERN9benchmark5StateE":
.LFB7852:
	push	rdi
	.seh_pushreg	rdi
	push	rsi
	.seh_pushreg	rsi
	push	rbx
	.seh_pushreg	rbx
	sub	rsp, 48
	.seh_stackalloc	48
	.seh_endprologue
	mov	eax, DWORD PTR 28[rcx]
	mov	rsi, rcx
	test	eax, eax
	je	.L41
	call	[QWORD PTR __imp__ZN9benchmark5State16StartKeepRunningEv[rip]]
.L42:
	mov	rcx, rsi
	add	rsp, 48
	pop	rbx
	pop	rsi
	pop	rdi
	rex.W jmp	[QWORD PTR __imp__ZN9benchmark5State17FinishKeepRunningEv[rip]]
	.p2align 4,,10
	.p2align 3
.L41:
	mov	rbx, QWORD PTR 16[rcx]
	call	[QWORD PTR __imp__ZN9benchmark5State16StartKeepRunningEv[rip]]
	test	rbx, rbx
	je	.L42
	jle	.L51
	.p2align 4
	.p2align 3
.L43:
	mov	QWORD PTR 32[rsp], 123456
	mov	QWORD PTR 40[rsp], 0
	sub	rbx, 1
	jne	.L43
	mov	rcx, rsi
	add	rsp, 48
	pop	rbx
	pop	rsi
	pop	rdi
	rex.W jmp	[QWORD PTR __imp__ZN9benchmark5State17FinishKeepRunningEv[rip]]
	.p2align 4,,10
	.p2align 3
.L51:
	mov	rdi, QWORD PTR __imp__assert[rip]
.L44:
	mov	QWORD PTR 32[rsp], 123456
	mov	QWORD PTR 40[rsp], 0
	mov	r8d, 257
	lea	rdx, .LC1[rip]
	lea	rcx, .LC2[rip]
	call	rdi
	sub	rbx, 1
	jne	.L44
	mov	rcx, rsi
	add	rsp, 48
	pop	rbx
	pop	rsi
	pop	rdi
	rex.W jmp	[QWORD PTR __imp__ZN9benchmark5State17FinishKeepRunningEv[rip]]
	.seh_endproc
	.p2align 4
	.def	"_ZN12_GLOBAL__N_1L16BM_RawArray_ReadERN9benchmark5StateE";	.scl	3;	.type	32;	.endef
	.seh_proc	"_ZN12_GLOBAL__N_1L16BM_RawArray_ReadERN9benchmark5StateE"
"_ZN12_GLOBAL__N_1L16BM_RawArray_ReadERN9benchmark5StateE":
.LFB7870:
	push	rdi
	.seh_pushreg	rdi
	push	rsi
	.seh_pushreg	rsi
	push	rbx
	.seh_pushreg	rbx
	sub	rsp, 288
	.seh_stackalloc	288
	.seh_endprologue
	pxor	xmm0, xmm0
	xor	eax, eax
	mov	rsi, rcx
.L53:
	mov	edx, eax
	add	eax, 64
	movups	XMMWORD PTR 32[rsp+rdx], xmm0
	movups	XMMWORD PTR 48[rsp+rdx], xmm0
	movups	XMMWORD PTR 64[rsp+rdx], xmm0
	movups	XMMWORD PTR 80[rsp+rdx], xmm0
	cmp	eax, 256
	jb	.L53
	mov	QWORD PTR 48[rsp], 123456
	mov	eax, DWORD PTR 28[rsi]
	mov	QWORD PTR 56[rsp], 0
	test	eax, eax
	jne	.L73
	mov	rbx, QWORD PTR 16[rsi]
	mov	rcx, rsi
	call	[QWORD PTR __imp__ZN9benchmark5State16StartKeepRunningEv[rip]]
	test	rbx, rbx
	je	.L56
	jle	.L71
	test	bl, 1
	je	.L57
	sub	rbx, 1
	je	.L56
	.p2align 4
	.p2align 3
.L57:
	sub	rbx, 2
	jne	.L57
.L56:
	mov	rcx, rsi
	call	[QWORD PTR __imp__ZN9benchmark5State17FinishKeepRunningEv[rip]]
	nop
	add	rsp, 288
	pop	rbx
	pop	rsi
	pop	rdi
	ret
	.p2align 4,,10
	.p2align 3
.L73:
	mov	rcx, rsi
	call	[QWORD PTR __imp__ZN9benchmark5State16StartKeepRunningEv[rip]]
	jmp	.L56
	.p2align 4,,10
	.p2align 3
.L71:
	mov	rdi, QWORD PTR __imp__assert[rip]
.L58:
	mov	r8d, 257
	lea	rdx, .LC1[rip]
	lea	rcx, .LC2[rip]
	call	rdi
	sub	rbx, 1
	jne	.L58
	jmp	.L56
	.seh_endproc
	.p2align 4
	.def	"_ZN12_GLOBAL__N_1L17BM_RawArray_WriteERN9benchmark5StateE";	.scl	3;	.type	32;	.endef
	.seh_proc	"_ZN12_GLOBAL__N_1L17BM_RawArray_WriteERN9benchmark5StateE"
"_ZN12_GLOBAL__N_1L17BM_RawArray_WriteERN9benchmark5StateE":
.LFB7868:
	push	r13
	.seh_pushreg	r13
	push	r12
	.seh_pushreg	r12
	push	rbp
	.seh_pushreg	rbp
	push	rdi
	.seh_pushreg	rdi
	push	rsi
	.seh_pushreg	rsi
	push	rbx
	.seh_pushreg	rbx
	sub	rsp, 296
	.seh_stackalloc	296
	.seh_endprologue
	pxor	xmm0, xmm0
	xor	eax, eax
	mov	rbx, rcx
.L75:
	mov	edx, eax
	add	eax, 64
	movups	XMMWORD PTR 32[rsp+rdx], xmm0
	movups	XMMWORD PTR 48[rsp+rdx], xmm0
	movups	XMMWORD PTR 64[rsp+rdx], xmm0
	movups	XMMWORD PTR 80[rsp+rdx], xmm0
	cmp	eax, 256
	jb	.L75
	mov	eax, DWORD PTR 28[rbx]
	test	eax, eax
	jne	.L89
	mov	rsi, QWORD PTR 16[rbx]
	mov	rcx, rbx
	call	[QWORD PTR __imp__ZN9benchmark5State16StartKeepRunningEv[rip]]
	test	rsi, rsi
	je	.L78
	mov	edi, 0
	mov	eax, 0
	mov	edx, 0
	jle	.L90
	.p2align 4
	.p2align 3
.L81:
	mov	r8, rax
	mov	r9, rdx
	add	rax, 1
	mov	QWORD PTR 48[rsp], r8
	adc	rdx, 0
	mov	QWORD PTR 56[rsp], r9
	mov	rcx, rsi
	mov	r8, rdi
	xor	rcx, rax
	xor	r8, rdx
	or	rcx, r8
	jne	.L81
.L78:
	mov	rcx, rbx
	call	[QWORD PTR __imp__ZN9benchmark5State17FinishKeepRunningEv[rip]]
	nop
	add	rsp, 296
	pop	rbx
	pop	rsi
	pop	rdi
	pop	rbp
	pop	r12
	pop	r13
	ret
	.p2align 4,,10
	.p2align 3
.L89:
	mov	rcx, rbx
	call	[QWORD PTR __imp__ZN9benchmark5State16StartKeepRunningEv[rip]]
	jmp	.L78
	.p2align 4,,10
	.p2align 3
.L90:
	mov	rbp, QWORD PTR __imp__assert[rip]
	xor	r12d, r12d
	xor	r13d, r13d
.L80:
	mov	rax, r12
	mov	rdx, r13
	add	r12, 1
	mov	QWORD PTR 48[rsp], rax
	adc	r13, 0
	mov	QWORD PTR 56[rsp], rdx
	lea	rdx, .LC1[rip]
	mov	r8d, 257
	lea	rcx, .LC2[rip]
	call	rbp
	mov	rax, r12
	mov	rdx, r13
	xor	rax, rsi
	xor	rdx, rdi
	or	rax, rdx
	jne	.L80
	jmp	.L78
	.seh_endproc
	.p2align 4
	.def	"_ZN12_GLOBAL__N_1L21BM_RegisterFile_ResetERN9benchmark5StateE";	.scl	3;	.type	32;	.endef
	.seh_proc	"_ZN12_GLOBAL__N_1L21BM_RegisterFile_ResetERN9benchmark5StateE"
"_ZN12_GLOBAL__N_1L21BM_RegisterFile_ResetERN9benchmark5StateE":
.LFB7865:
	push	rdi
	.seh_pushreg	rdi
	push	rsi
	.seh_pushreg	rsi
	push	rbx
	.seh_pushreg	rbx
	sub	rsp, 560
	.seh_stackalloc	560
	movups	XMMWORD PTR 544[rsp], xmm6
	.seh_savexmm	xmm6, 544
	.seh_endprologue
	mov	r8d, 512
	lea	rdx, .LC0[rip]
	mov	rsi, rcx
	lea	rcx, 32[rsp]
	call	"memcpy"
	mov	eax, DWORD PTR 28[rsi]
	test	eax, eax
	je	.L92
	mov	rcx, rsi
	call	[QWORD PTR __imp__ZN9benchmark5State16StartKeepRunningEv[rip]]
.L93:
	mov	rcx, rsi
	call	[QWORD PTR __imp__ZN9benchmark5State17FinishKeepRunningEv[rip]]
	nop
	movups	xmm6, XMMWORD PTR 544[rsp]
	add	rsp, 560
	pop	rbx
	pop	rsi
	pop	rdi
	ret
.L92:
	mov	rbx, QWORD PTR 16[rsi]
	mov	rcx, rsi
	call	[QWORD PTR __imp__ZN9benchmark5State16StartKeepRunningEv[rip]]
	test	rbx, rbx
	je	.L93
	jle	.L102
	.p2align 4
	.p2align 3
.L94:
	pxor	xmm0, xmm0
	movups	XMMWORD PTR 48[rsp], xmm0
	movups	XMMWORD PTR 80[rsp], xmm0
	movups	XMMWORD PTR 112[rsp], xmm0
	movups	XMMWORD PTR 144[rsp], xmm0
	movups	XMMWORD PTR 176[rsp], xmm0
	movups	XMMWORD PTR 208[rsp], xmm0
	movups	XMMWORD PTR 240[rsp], xmm0
	movups	XMMWORD PTR 272[rsp], xmm0
	movups	XMMWORD PTR 304[rsp], xmm0
	movups	XMMWORD PTR 336[rsp], xmm0
	movups	XMMWORD PTR 368[rsp], xmm0
	movups	XMMWORD PTR 400[rsp], xmm0
	movups	XMMWORD PTR 432[rsp], xmm0
	movups	XMMWORD PTR 464[rsp], xmm0
	movups	XMMWORD PTR 496[rsp], xmm0
	movups	XMMWORD PTR 528[rsp], xmm0
	sub	rbx, 1
	jne	.L94
	jmp	.L93
.L102:
	mov	rdi, QWORD PTR __imp__assert[rip]
.L95:
	pxor	xmm0, xmm0
	movups	XMMWORD PTR 48[rsp], xmm0
	movups	XMMWORD PTR 80[rsp], xmm0
	movups	XMMWORD PTR 112[rsp], xmm0
	movups	XMMWORD PTR 144[rsp], xmm0
	movups	XMMWORD PTR 176[rsp], xmm0
	movups	XMMWORD PTR 208[rsp], xmm0
	movups	XMMWORD PTR 240[rsp], xmm0
	movups	XMMWORD PTR 272[rsp], xmm0
	movups	XMMWORD PTR 304[rsp], xmm0
	movups	XMMWORD PTR 336[rsp], xmm0
	movups	XMMWORD PTR 368[rsp], xmm0
	movups	XMMWORD PTR 400[rsp], xmm0
	movups	XMMWORD PTR 432[rsp], xmm0
	movups	XMMWORD PTR 464[rsp], xmm0
	movups	XMMWORD PTR 496[rsp], xmm0
	movups	XMMWORD PTR 528[rsp], xmm0
	mov	r8d, 257
	lea	rdx, .LC1[rip]
	lea	rcx, .LC2[rip]
	call	rdi
	sub	rbx, 1
	jne	.L95
	jmp	.L93
	.seh_endproc
	.p2align 4
	.def	"_ZN12_GLOBAL__N_1L24BM_RegisterFile_EnumReadERN9benchmark5StateE";	.scl	3;	.type	32;	.endef
	.seh_proc	"_ZN12_GLOBAL__N_1L24BM_RegisterFile_EnumReadERN9benchmark5StateE"
"_ZN12_GLOBAL__N_1L24BM_RegisterFile_EnumReadERN9benchmark5StateE":
.LFB10770:
	push	rdi
	.seh_pushreg	rdi
	push	rsi
	.seh_pushreg	rsi
	push	rbx
	.seh_pushreg	rbx
	sub	rsp, 48
	.seh_stackalloc	48
	.seh_endprologue
	mov	eax, DWORD PTR 28[rcx]
	mov	rsi, rcx
	test	eax, eax
	je	.L104
	call	[QWORD PTR __imp__ZN9benchmark5State16StartKeepRunningEv[rip]]
.L105:
	mov	rcx, rsi
	add	rsp, 48
	pop	rbx
	pop	rsi
	pop	rdi
	rex.W jmp	[QWORD PTR __imp__ZN9benchmark5State17FinishKeepRunningEv[rip]]
	.p2align 4,,10
	.p2align 3
.L104:
	mov	rbx, QWORD PTR 16[rcx]
	call	[QWORD PTR __imp__ZN9benchmark5State16StartKeepRunningEv[rip]]
	test	rbx, rbx
	je	.L105
	jle	.L114
	.p2align 4
	.p2align 3
.L106:
	mov	QWORD PTR 32[rsp], 123456
	mov	QWORD PTR 40[rsp], 0
	sub	rbx, 1
	jne	.L106
	mov	rcx, rsi
	add	rsp, 48
	pop	rbx
	pop	rsi
	pop	rdi
	rex.W jmp	[QWORD PTR __imp__ZN9benchmark5State17FinishKeepRunningEv[rip]]
	.p2align 4,,10
	.p2align 3
.L114:
	mov	rdi, QWORD PTR __imp__assert[rip]
.L107:
	mov	QWORD PTR 32[rsp], 123456
	mov	QWORD PTR 40[rsp], 0
	mov	r8d, 257
	lea	rdx, .LC1[rip]
	lea	rcx, .LC2[rip]
	call	rdi
	sub	rbx, 1
	jne	.L107
	mov	rcx, rsi
	add	rsp, 48
	pop	rbx
	pop	rsi
	pop	rdi
	rex.W jmp	[QWORD PTR __imp__ZN9benchmark5State17FinishKeepRunningEv[rip]]
	.seh_endproc
	.section	.text.startup,"x"
	.p2align 4
	.def	"_ZSt11make_uniqueIN9benchmark8internal17FunctionBenchmarkEJRA27_KcPFvRNS0_5StateEEEENSt8__detail9_MakeUniqIT_E15__single_objectEDpOT0_.isra.0";	.scl	3;	.type	32;	.endef
	.seh_proc	"_ZSt11make_uniqueIN9benchmark8internal17FunctionBenchmarkEJRA27_KcPFvRNS0_5StateEEEENSt8__detail9_MakeUniqIT_E15__single_objectEDpOT0_.isra.0"
"_ZSt11make_uniqueIN9benchmark8internal17FunctionBenchmarkEJRA27_KcPFvRNS0_5StateEEEENSt8__detail9_MakeUniqIT_E15__single_objectEDpOT0_.isra.0":
.LFB10772:
	push	r13
	.seh_pushreg	r13
	push	r12
	.seh_pushreg	r12
	push	rbp
	.seh_pushreg	rbp
	push	rdi
	.seh_pushreg	rdi
	push	rsi
	.seh_pushreg	rsi
	push	rbx
	.seh_pushreg	rbx
	sub	rsp, 72
	.seh_stackalloc	72
	.seh_endprologue
	mov	rbp, rdx
	mov	rdi, rcx
	mov	ecx, 312
	mov	r12, r8
.LEHB0:
	call	"_Znwy"
.LEHE0:
	lea	r13, 48[rsp]
	pxor	xmm0, xmm0
	mov	rcx, rbp
	movups	XMMWORD PTR 32[rsp], xmm0
	mov	rsi, rax
	mov	QWORD PTR 32[rsp], r13
	movups	XMMWORD PTR 48[rsp], xmm0
	call	"strlen"
	mov	rbx, rax
	cmp	rax, 15
	ja	.L129
	cmp	rax, 1
	je	.L130
	test	rax, rax
	jne	.L131
.L119:
	mov	rax, QWORD PTR 32[rsp]
	lea	rdx, 32[rsp]
	mov	rcx, rsi
	mov	BYTE PTR [rax+rbx], 0
	mov	QWORD PTR 40[rsp], rbx
.LEHB1:
	call	[QWORD PTR __imp__ZN9benchmark9BenchmarkC2ERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE[rip]]
.LEHE1:
	mov	rax, QWORD PTR __imp__ZTVN9benchmark8internal17FunctionBenchmarkE[rip]
	mov	rcx, QWORD PTR 32[rsp]
	mov	QWORD PTR 304[rsi], r12
	mov	QWORD PTR [rdi], rsi
	add	rax, 16
	mov	QWORD PTR [rsi], rax
	cmp	rcx, r13
	je	.L115
	mov	rax, QWORD PTR 48[rsp]
	lea	rdx, 1[rax]
	call	"_ZdlPvy"
.L115:
	mov	rax, rdi
	add	rsp, 72
	pop	rbx
	pop	rsi
	pop	rdi
	pop	rbp
	pop	r12
	pop	r13
	ret
.L130:
	movzx	eax, BYTE PTR 0[rbp]
	mov	BYTE PTR 48[rsp], al
	jmp	.L119
.L129:
	lea	rcx, 1[rax]
.LEHB2:
	call	"_Znwy"
.LEHE2:
	mov	QWORD PTR 32[rsp], rax
	mov	rcx, rax
	mov	QWORD PTR 48[rsp], rbx
.L117:
	mov	r8, rbx
	mov	rdx, rbp
	call	"memcpy"
	jmp	.L119
.L131:
	mov	rcx, r13
	jmp	.L117
.L124:
	mov	rbx, rax
.L123:
	mov	rcx, rsi
	mov	edx, 312
	call	"_ZdlPvy"
	mov	rcx, rbx
.LEHB3:
	call	"_Unwind_Resume"
.LEHE3:
.L125:
	mov	rcx, QWORD PTR 32[rsp]
	mov	rbx, rax
	cmp	rcx, r13
	je	.L123
	mov	rax, QWORD PTR 48[rsp]
	lea	rdx, 1[rax]
	call	"_ZdlPvy"
	jmp	.L123
	.seh_handler	"__gxx_personality_seh0", @unwind, @except
	.seh_handlerdata
.LLSDA10772:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSE10772-.LLSDACSB10772
.LLSDACSB10772:
	.uleb128 .LEHB0-.LFB10772
	.uleb128 .LEHE0-.LEHB0
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB1-.LFB10772
	.uleb128 .LEHE1-.LEHB1
	.uleb128 .L125-.LFB10772
	.uleb128 0
	.uleb128 .LEHB2-.LFB10772
	.uleb128 .LEHE2-.LEHB2
	.uleb128 .L124-.LFB10772
	.uleb128 0
	.uleb128 .LEHB3-.LFB10772
	.uleb128 .LEHE3-.LEHB3
	.uleb128 0
	.uleb128 0
.LLSDACSE10772:
	.section	.text.startup,"x"
	.seh_endproc
	.p2align 4
	.def	"_ZSt11make_uniqueIN9benchmark8internal17FunctionBenchmarkEJRA25_KcPFvRNS0_5StateEEEENSt8__detail9_MakeUniqIT_E15__single_objectEDpOT0_.isra.0";	.scl	3;	.type	32;	.endef
	.seh_proc	"_ZSt11make_uniqueIN9benchmark8internal17FunctionBenchmarkEJRA25_KcPFvRNS0_5StateEEEENSt8__detail9_MakeUniqIT_E15__single_objectEDpOT0_.isra.0"
"_ZSt11make_uniqueIN9benchmark8internal17FunctionBenchmarkEJRA25_KcPFvRNS0_5StateEEEENSt8__detail9_MakeUniqIT_E15__single_objectEDpOT0_.isra.0":
.LFB10779:
	push	r13
	.seh_pushreg	r13
	push	r12
	.seh_pushreg	r12
	push	rbp
	.seh_pushreg	rbp
	push	rdi
	.seh_pushreg	rdi
	push	rsi
	.seh_pushreg	rsi
	push	rbx
	.seh_pushreg	rbx
	sub	rsp, 72
	.seh_stackalloc	72
	.seh_endprologue
	mov	rbp, rdx
	mov	rdi, rcx
	mov	ecx, 312
	mov	r12, r8
.LEHB4:
	call	"_Znwy"
.LEHE4:
	lea	r13, 48[rsp]
	pxor	xmm0, xmm0
	mov	rcx, rbp
	movups	XMMWORD PTR 32[rsp], xmm0
	mov	rsi, rax
	mov	QWORD PTR 32[rsp], r13
	movups	XMMWORD PTR 48[rsp], xmm0
	call	"strlen"
	mov	rbx, rax
	cmp	rax, 15
	ja	.L146
	cmp	rax, 1
	je	.L147
	test	rax, rax
	jne	.L148
.L136:
	mov	rax, QWORD PTR 32[rsp]
	lea	rdx, 32[rsp]
	mov	rcx, rsi
	mov	BYTE PTR [rax+rbx], 0
	mov	QWORD PTR 40[rsp], rbx
.LEHB5:
	call	[QWORD PTR __imp__ZN9benchmark9BenchmarkC2ERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE[rip]]
.LEHE5:
	mov	rax, QWORD PTR __imp__ZTVN9benchmark8internal17FunctionBenchmarkE[rip]
	mov	rcx, QWORD PTR 32[rsp]
	mov	QWORD PTR 304[rsi], r12
	mov	QWORD PTR [rdi], rsi
	add	rax, 16
	mov	QWORD PTR [rsi], rax
	cmp	rcx, r13
	je	.L132
	mov	rax, QWORD PTR 48[rsp]
	lea	rdx, 1[rax]
	call	"_ZdlPvy"
.L132:
	mov	rax, rdi
	add	rsp, 72
	pop	rbx
	pop	rsi
	pop	rdi
	pop	rbp
	pop	r12
	pop	r13
	ret
.L147:
	movzx	eax, BYTE PTR 0[rbp]
	mov	BYTE PTR 48[rsp], al
	jmp	.L136
.L146:
	lea	rcx, 1[rax]
.LEHB6:
	call	"_Znwy"
.LEHE6:
	mov	QWORD PTR 32[rsp], rax
	mov	rcx, rax
	mov	QWORD PTR 48[rsp], rbx
.L134:
	mov	r8, rbx
	mov	rdx, rbp
	call	"memcpy"
	jmp	.L136
.L148:
	mov	rcx, r13
	jmp	.L134
.L141:
	mov	rbx, rax
.L140:
	mov	rcx, rsi
	mov	edx, 312
	call	"_ZdlPvy"
	mov	rcx, rbx
.LEHB7:
	call	"_Unwind_Resume"
.LEHE7:
.L142:
	mov	rcx, QWORD PTR 32[rsp]
	mov	rbx, rax
	cmp	rcx, r13
	je	.L140
	mov	rax, QWORD PTR 48[rsp]
	lea	rdx, 1[rax]
	call	"_ZdlPvy"
	jmp	.L140
	.seh_handler	"__gxx_personality_seh0", @unwind, @except
	.seh_handlerdata
.LLSDA10779:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSE10779-.LLSDACSB10779
.LLSDACSB10779:
	.uleb128 .LEHB4-.LFB10779
	.uleb128 .LEHE4-.LEHB4
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB5-.LFB10779
	.uleb128 .LEHE5-.LEHB5
	.uleb128 .L142-.LFB10779
	.uleb128 0
	.uleb128 .LEHB6-.LFB10779
	.uleb128 .LEHE6-.LEHB6
	.uleb128 .L141-.LFB10779
	.uleb128 0
	.uleb128 .LEHB7-.LFB10779
	.uleb128 .LEHE7-.LEHB7
	.uleb128 0
	.uleb128 0
.LLSDACSE10779:
	.section	.text.startup,"x"
	.seh_endproc
	.p2align 4
	.def	"_ZSt11make_uniqueIN9benchmark8internal17FunctionBenchmarkEJRA26_KcPFvRNS0_5StateEEEENSt8__detail9_MakeUniqIT_E15__single_objectEDpOT0_.isra.0";	.scl	3;	.type	32;	.endef
	.seh_proc	"_ZSt11make_uniqueIN9benchmark8internal17FunctionBenchmarkEJRA26_KcPFvRNS0_5StateEEEENSt8__detail9_MakeUniqIT_E15__single_objectEDpOT0_.isra.0"
"_ZSt11make_uniqueIN9benchmark8internal17FunctionBenchmarkEJRA26_KcPFvRNS0_5StateEEEENSt8__detail9_MakeUniqIT_E15__single_objectEDpOT0_.isra.0":
.LFB10780:
	push	r13
	.seh_pushreg	r13
	push	r12
	.seh_pushreg	r12
	push	rbp
	.seh_pushreg	rbp
	push	rdi
	.seh_pushreg	rdi
	push	rsi
	.seh_pushreg	rsi
	push	rbx
	.seh_pushreg	rbx
	sub	rsp, 72
	.seh_stackalloc	72
	.seh_endprologue
	mov	rbp, rdx
	mov	rdi, rcx
	mov	ecx, 312
	mov	r12, r8
.LEHB8:
	call	"_Znwy"
.LEHE8:
	lea	r13, 48[rsp]
	pxor	xmm0, xmm0
	mov	rcx, rbp
	movups	XMMWORD PTR 32[rsp], xmm0
	mov	rsi, rax
	mov	QWORD PTR 32[rsp], r13
	movups	XMMWORD PTR 48[rsp], xmm0
	call	"strlen"
	mov	rbx, rax
	cmp	rax, 15
	ja	.L163
	cmp	rax, 1
	je	.L164
	test	rax, rax
	jne	.L165
.L153:
	mov	rax, QWORD PTR 32[rsp]
	lea	rdx, 32[rsp]
	mov	rcx, rsi
	mov	BYTE PTR [rax+rbx], 0
	mov	QWORD PTR 40[rsp], rbx
.LEHB9:
	call	[QWORD PTR __imp__ZN9benchmark9BenchmarkC2ERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE[rip]]
.LEHE9:
	mov	rax, QWORD PTR __imp__ZTVN9benchmark8internal17FunctionBenchmarkE[rip]
	mov	rcx, QWORD PTR 32[rsp]
	mov	QWORD PTR 304[rsi], r12
	mov	QWORD PTR [rdi], rsi
	add	rax, 16
	mov	QWORD PTR [rsi], rax
	cmp	rcx, r13
	je	.L149
	mov	rax, QWORD PTR 48[rsp]
	lea	rdx, 1[rax]
	call	"_ZdlPvy"
.L149:
	mov	rax, rdi
	add	rsp, 72
	pop	rbx
	pop	rsi
	pop	rdi
	pop	rbp
	pop	r12
	pop	r13
	ret
.L164:
	movzx	eax, BYTE PTR 0[rbp]
	mov	BYTE PTR 48[rsp], al
	jmp	.L153
.L163:
	lea	rcx, 1[rax]
.LEHB10:
	call	"_Znwy"
.LEHE10:
	mov	QWORD PTR 32[rsp], rax
	mov	rcx, rax
	mov	QWORD PTR 48[rsp], rbx
.L151:
	mov	r8, rbx
	mov	rdx, rbp
	call	"memcpy"
	jmp	.L153
.L165:
	mov	rcx, r13
	jmp	.L151
.L158:
	mov	rbx, rax
.L157:
	mov	rcx, rsi
	mov	edx, 312
	call	"_ZdlPvy"
	mov	rcx, rbx
.LEHB11:
	call	"_Unwind_Resume"
.LEHE11:
.L159:
	mov	rcx, QWORD PTR 32[rsp]
	mov	rbx, rax
	cmp	rcx, r13
	je	.L157
	mov	rax, QWORD PTR 48[rsp]
	lea	rdx, 1[rax]
	call	"_ZdlPvy"
	jmp	.L157
	.seh_handler	"__gxx_personality_seh0", @unwind, @except
	.seh_handlerdata
.LLSDA10780:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSE10780-.LLSDACSB10780
.LLSDACSB10780:
	.uleb128 .LEHB8-.LFB10780
	.uleb128 .LEHE8-.LEHB8
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB9-.LFB10780
	.uleb128 .LEHE9-.LEHB9
	.uleb128 .L159-.LFB10780
	.uleb128 0
	.uleb128 .LEHB10-.LFB10780
	.uleb128 .LEHE10-.LEHB10
	.uleb128 .L158-.LFB10780
	.uleb128 0
	.uleb128 .LEHB11-.LFB10780
	.uleb128 .LEHE11-.LEHB11
	.uleb128 0
	.uleb128 0
.LLSDACSE10780:
	.section	.text.startup,"x"
	.seh_endproc
	.p2align 4
	.def	"_ZSt11make_uniqueIN9benchmark8internal17FunctionBenchmarkEJRA17_KcPFvRNS0_5StateEEEENSt8__detail9_MakeUniqIT_E15__single_objectEDpOT0_.isra.0";	.scl	3;	.type	32;	.endef
	.seh_proc	"_ZSt11make_uniqueIN9benchmark8internal17FunctionBenchmarkEJRA17_KcPFvRNS0_5StateEEEENSt8__detail9_MakeUniqIT_E15__single_objectEDpOT0_.isra.0"
"_ZSt11make_uniqueIN9benchmark8internal17FunctionBenchmarkEJRA17_KcPFvRNS0_5StateEEEENSt8__detail9_MakeUniqIT_E15__single_objectEDpOT0_.isra.0":
.LFB10781:
	push	r13
	.seh_pushreg	r13
	push	r12
	.seh_pushreg	r12
	push	rbp
	.seh_pushreg	rbp
	push	rdi
	.seh_pushreg	rdi
	push	rsi
	.seh_pushreg	rsi
	push	rbx
	.seh_pushreg	rbx
	sub	rsp, 72
	.seh_stackalloc	72
	.seh_endprologue
	mov	rbp, rdx
	mov	rdi, rcx
	mov	ecx, 312
	mov	r12, r8
.LEHB12:
	call	"_Znwy"
.LEHE12:
	lea	r13, 48[rsp]
	pxor	xmm0, xmm0
	mov	rcx, rbp
	movups	XMMWORD PTR 32[rsp], xmm0
	mov	rsi, rax
	mov	QWORD PTR 32[rsp], r13
	movups	XMMWORD PTR 48[rsp], xmm0
	call	"strlen"
	mov	rbx, rax
	cmp	rax, 15
	ja	.L180
	cmp	rax, 1
	je	.L181
	test	rax, rax
	jne	.L182
.L170:
	mov	rax, QWORD PTR 32[rsp]
	lea	rdx, 32[rsp]
	mov	rcx, rsi
	mov	BYTE PTR [rax+rbx], 0
	mov	QWORD PTR 40[rsp], rbx
.LEHB13:
	call	[QWORD PTR __imp__ZN9benchmark9BenchmarkC2ERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE[rip]]
.LEHE13:
	mov	rax, QWORD PTR __imp__ZTVN9benchmark8internal17FunctionBenchmarkE[rip]
	mov	rcx, QWORD PTR 32[rsp]
	mov	QWORD PTR 304[rsi], r12
	mov	QWORD PTR [rdi], rsi
	add	rax, 16
	mov	QWORD PTR [rsi], rax
	cmp	rcx, r13
	je	.L166
	mov	rax, QWORD PTR 48[rsp]
	lea	rdx, 1[rax]
	call	"_ZdlPvy"
.L166:
	mov	rax, rdi
	add	rsp, 72
	pop	rbx
	pop	rsi
	pop	rdi
	pop	rbp
	pop	r12
	pop	r13
	ret
.L181:
	movzx	eax, BYTE PTR 0[rbp]
	mov	BYTE PTR 48[rsp], al
	jmp	.L170
.L180:
	lea	rcx, 1[rax]
.LEHB14:
	call	"_Znwy"
.LEHE14:
	mov	QWORD PTR 32[rsp], rax
	mov	rcx, rax
	mov	QWORD PTR 48[rsp], rbx
.L168:
	mov	r8, rbx
	mov	rdx, rbp
	call	"memcpy"
	jmp	.L170
.L182:
	mov	rcx, r13
	jmp	.L168
.L175:
	mov	rbx, rax
.L174:
	mov	rcx, rsi
	mov	edx, 312
	call	"_ZdlPvy"
	mov	rcx, rbx
.LEHB15:
	call	"_Unwind_Resume"
.LEHE15:
.L176:
	mov	rcx, QWORD PTR 32[rsp]
	mov	rbx, rax
	cmp	rcx, r13
	je	.L174
	mov	rax, QWORD PTR 48[rsp]
	lea	rdx, 1[rax]
	call	"_ZdlPvy"
	jmp	.L174
	.seh_handler	"__gxx_personality_seh0", @unwind, @except
	.seh_handlerdata
.LLSDA10781:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSE10781-.LLSDACSB10781
.LLSDACSB10781:
	.uleb128 .LEHB12-.LFB10781
	.uleb128 .LEHE12-.LEHB12
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB13-.LFB10781
	.uleb128 .LEHE13-.LEHB13
	.uleb128 .L176-.LFB10781
	.uleb128 0
	.uleb128 .LEHB14-.LFB10781
	.uleb128 .LEHE14-.LEHB14
	.uleb128 .L175-.LFB10781
	.uleb128 0
	.uleb128 .LEHB15-.LFB10781
	.uleb128 .LEHE15-.LEHB15
	.uleb128 0
	.uleb128 0
.LLSDACSE10781:
	.section	.text.startup,"x"
	.seh_endproc
	.p2align 4
	.def	"_ZSt11make_uniqueIN9benchmark8internal17FunctionBenchmarkEJRA18_KcPFvRNS0_5StateEEEENSt8__detail9_MakeUniqIT_E15__single_objectEDpOT0_.isra.0";	.scl	3;	.type	32;	.endef
	.seh_proc	"_ZSt11make_uniqueIN9benchmark8internal17FunctionBenchmarkEJRA18_KcPFvRNS0_5StateEEEENSt8__detail9_MakeUniqIT_E15__single_objectEDpOT0_.isra.0"
"_ZSt11make_uniqueIN9benchmark8internal17FunctionBenchmarkEJRA18_KcPFvRNS0_5StateEEEENSt8__detail9_MakeUniqIT_E15__single_objectEDpOT0_.isra.0":
.LFB10782:
	push	r13
	.seh_pushreg	r13
	push	r12
	.seh_pushreg	r12
	push	rbp
	.seh_pushreg	rbp
	push	rdi
	.seh_pushreg	rdi
	push	rsi
	.seh_pushreg	rsi
	push	rbx
	.seh_pushreg	rbx
	sub	rsp, 72
	.seh_stackalloc	72
	.seh_endprologue
	mov	rbp, rdx
	mov	rdi, rcx
	mov	ecx, 312
	mov	r12, r8
.LEHB16:
	call	"_Znwy"
.LEHE16:
	lea	r13, 48[rsp]
	pxor	xmm0, xmm0
	mov	rcx, rbp
	movups	XMMWORD PTR 32[rsp], xmm0
	mov	rsi, rax
	mov	QWORD PTR 32[rsp], r13
	movups	XMMWORD PTR 48[rsp], xmm0
	call	"strlen"
	mov	rbx, rax
	cmp	rax, 15
	ja	.L197
	cmp	rax, 1
	je	.L198
	test	rax, rax
	jne	.L199
.L187:
	mov	rax, QWORD PTR 32[rsp]
	lea	rdx, 32[rsp]
	mov	rcx, rsi
	mov	BYTE PTR [rax+rbx], 0
	mov	QWORD PTR 40[rsp], rbx
.LEHB17:
	call	[QWORD PTR __imp__ZN9benchmark9BenchmarkC2ERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE[rip]]
.LEHE17:
	mov	rax, QWORD PTR __imp__ZTVN9benchmark8internal17FunctionBenchmarkE[rip]
	mov	rcx, QWORD PTR 32[rsp]
	mov	QWORD PTR 304[rsi], r12
	mov	QWORD PTR [rdi], rsi
	add	rax, 16
	mov	QWORD PTR [rsi], rax
	cmp	rcx, r13
	je	.L183
	mov	rax, QWORD PTR 48[rsp]
	lea	rdx, 1[rax]
	call	"_ZdlPvy"
.L183:
	mov	rax, rdi
	add	rsp, 72
	pop	rbx
	pop	rsi
	pop	rdi
	pop	rbp
	pop	r12
	pop	r13
	ret
.L198:
	movzx	eax, BYTE PTR 0[rbp]
	mov	BYTE PTR 48[rsp], al
	jmp	.L187
.L197:
	lea	rcx, 1[rax]
.LEHB18:
	call	"_Znwy"
.LEHE18:
	mov	QWORD PTR 32[rsp], rax
	mov	rcx, rax
	mov	QWORD PTR 48[rsp], rbx
.L185:
	mov	r8, rbx
	mov	rdx, rbp
	call	"memcpy"
	jmp	.L187
.L199:
	mov	rcx, r13
	jmp	.L185
.L192:
	mov	rbx, rax
.L191:
	mov	rcx, rsi
	mov	edx, 312
	call	"_ZdlPvy"
	mov	rcx, rbx
.LEHB19:
	call	"_Unwind_Resume"
.LEHE19:
.L193:
	mov	rcx, QWORD PTR 32[rsp]
	mov	rbx, rax
	cmp	rcx, r13
	je	.L191
	mov	rax, QWORD PTR 48[rsp]
	lea	rdx, 1[rax]
	call	"_ZdlPvy"
	jmp	.L191
	.seh_handler	"__gxx_personality_seh0", @unwind, @except
	.seh_handlerdata
.LLSDA10782:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSE10782-.LLSDACSB10782
.LLSDACSB10782:
	.uleb128 .LEHB16-.LFB10782
	.uleb128 .LEHE16-.LEHB16
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB17-.LFB10782
	.uleb128 .LEHE17-.LEHB17
	.uleb128 .L193-.LFB10782
	.uleb128 0
	.uleb128 .LEHB18-.LFB10782
	.uleb128 .LEHE18-.LEHB18
	.uleb128 .L192-.LFB10782
	.uleb128 0
	.uleb128 .LEHB19-.LFB10782
	.uleb128 .LEHE19-.LEHB19
	.uleb128 0
	.uleb128 0
.LLSDACSE10782:
	.section	.text.startup,"x"
	.seh_endproc
	.text
	.p2align 4
	.def	"_ZN12_GLOBAL__N_1L27BM_RegisterFile_StringWriteERN9benchmark5StateE";	.scl	3;	.type	32;	.endef
	.seh_proc	"_ZN12_GLOBAL__N_1L27BM_RegisterFile_StringWriteERN9benchmark5StateE"
"_ZN12_GLOBAL__N_1L27BM_RegisterFile_StringWriteERN9benchmark5StateE":
.LFB7861:
	push	r13
	.seh_pushreg	r13
	push	r12
	.seh_pushreg	r12
	push	rbp
	.seh_pushreg	rbp
	push	rdi
	.seh_pushreg	rdi
	push	rsi
	.seh_pushreg	rsi
	push	rbx
	.seh_pushreg	rbx
	sub	rsp, 552
	.seh_stackalloc	552
	.seh_endprologue
	mov	r8d, 512
	lea	rdx, .LC0[rip]
	mov	rbx, rcx
	lea	rcx, 32[rsp]
	call	"memcpy"
	mov	eax, DWORD PTR 28[rbx]
	test	eax, eax
	je	.L201
	mov	rcx, rbx
	call	[QWORD PTR __imp__ZN9benchmark5State16StartKeepRunningEv[rip]]
.L202:
	mov	rcx, rbx
	call	[QWORD PTR __imp__ZN9benchmark5State17FinishKeepRunningEv[rip]]
	nop
	add	rsp, 552
	pop	rbx
	pop	rsi
	pop	rdi
	pop	rbp
	pop	r12
	pop	r13
	ret
	.p2align 4,,10
	.p2align 3
.L201:
	mov	rsi, QWORD PTR 16[rbx]
	mov	rcx, rbx
	call	[QWORD PTR __imp__ZN9benchmark5State16StartKeepRunningEv[rip]]
	test	rsi, rsi
	je	.L202
	mov	edi, 0
	mov	eax, 0
	mov	edx, 0
	jle	.L212
	.p2align 4
	.p2align 3
.L205:
	mov	r8, rax
	add	rax, 1
	mov	r9, rdx
	adc	rdx, 0
	and	r8, QWORD PTR 64[rsp]
	and	r9, QWORD PTR 72[rsp]
	mov	QWORD PTR 80[rsp], r8
	mov	QWORD PTR 88[rsp], r9
	mov	rcx, rax
	mov	r8, rdx
	xor	rcx, rsi
	xor	r8, rdi
	or	rcx, r8
	jne	.L205
	jmp	.L202
.L212:
	mov	rbp, QWORD PTR __imp__assert[rip]
	xor	r12d, r12d
	xor	r13d, r13d
	.p2align 4
	.p2align 3
.L204:
	mov	rax, r12
	add	r12, 1
	mov	rdx, r13
	adc	r13, 0
	and	rax, QWORD PTR 64[rsp]
	and	rdx, QWORD PTR 72[rsp]
	mov	QWORD PTR 80[rsp], rax
	mov	QWORD PTR 88[rsp], rdx
	lea	rdx, .LC1[rip]
	mov	r8d, 257
	lea	rcx, .LC2[rip]
	call	rbp
	mov	rax, rsi
	mov	rdx, rdi
	xor	rax, r12
	xor	rdx, r13
	or	rax, rdx
	jne	.L204
	jmp	.L202
	.seh_endproc
	.section .rdata,"dr"
.LC3:
	.ascii "BM_Register_Write\0"
.LC4:
	.ascii "BM_Register_Read\0"
.LC5:
	.ascii "BM_Register_Copy\0"
.LC6:
	.ascii "BM_RegisterFile_EnumWrite\0"
.LC7:
	.ascii "BM_RegisterFile_EnumRead\0"
.LC8:
	.ascii "BM_RegisterFile_StringRead\0"
.LC9:
	.ascii "BM_RegisterFile_Arithmetic\0"
.LC10:
	.ascii "BM_RawArray_Write\0"
.LC11:
	.ascii "BM_RawArray_Read\0"
	.section	.text.startup,"x"
	.p2align 4
	.def	"_Z41__static_initialization_and_destruction_0v";	.scl	3;	.type	32;	.endef
	.seh_proc	"_Z41__static_initialization_and_destruction_0v"
"_Z41__static_initialization_and_destruction_0v":
.LFB9654:
	push	r12
	.seh_pushreg	r12
	push	rbp
	.seh_pushreg	rbp
	push	rdi
	.seh_pushreg	rdi
	push	rsi
	.seh_pushreg	rsi
	push	rbx
	.seh_pushreg	rbx
	sub	rsp, 96
	.seh_stackalloc	96
	movups	XMMWORD PTR 80[rsp], xmm6
	.seh_savexmm	xmm6, 80
	.seh_endprologue
.LEHB20:
	call	[QWORD PTR __imp__ZN9benchmark8internal17InitializeStreamsEv[rip]]
	lea	r8, "_ZN12_GLOBAL__N_1L17BM_Register_WriteERN9benchmark5StateE"[rip]
	lea	rdx, .LC3[rip]
	mov	QWORD PTR 48[rsp], 0
	lea	rcx, 40[rsp]
	call	"_ZSt11make_uniqueIN9benchmark8internal17FunctionBenchmarkEJRA18_KcPFvRNS0_5StateEEEENSt8__detail9_MakeUniqIT_E15__single_objectEDpOT0_.isra.0"
.LEHE20:
	mov	rax, QWORD PTR 40[rsp]
	lea	rcx, 48[rsp]
	mov	rbx, QWORD PTR __imp__ZN9benchmark8internal25RegisterBenchmarkInternalESt10unique_ptrINS_9BenchmarkESt14default_deleteIS2_EE[rip]
	mov	QWORD PTR 48[rsp], rax
.LEHB21:
	call	rbx
.LEHE21:
	mov	rcx, QWORD PTR 48[rsp]
	test	rcx, rcx
	je	.L214
	mov	rax, QWORD PTR [rcx]
	call	[QWORD PTR 8[rax]]
.L214:
	lea	rcx, 40[rsp]
	lea	r8, "_ZN12_GLOBAL__N_1L16BM_Register_ReadERN9benchmark5StateE"[rip]
	mov	QWORD PTR 48[rsp], 0
	lea	rdx, .LC4[rip]
.LEHB22:
	call	"_ZSt11make_uniqueIN9benchmark8internal17FunctionBenchmarkEJRA17_KcPFvRNS0_5StateEEEENSt8__detail9_MakeUniqIT_E15__single_objectEDpOT0_.isra.0"
.LEHE22:
	mov	rax, QWORD PTR 40[rsp]
	lea	rcx, 48[rsp]
	mov	QWORD PTR 48[rsp], rax
.LEHB23:
	call	rbx
.LEHE23:
	mov	rcx, QWORD PTR 48[rsp]
	test	rcx, rcx
	je	.L215
	mov	rax, QWORD PTR [rcx]
	call	[QWORD PTR 8[rax]]
.L215:
	lea	rcx, 40[rsp]
	lea	r8, "_ZN12_GLOBAL__N_1L16BM_Register_CopyERN9benchmark5StateE"[rip]
	mov	QWORD PTR 48[rsp], 0
	lea	rdx, .LC5[rip]
.LEHB24:
	call	"_ZSt11make_uniqueIN9benchmark8internal17FunctionBenchmarkEJRA17_KcPFvRNS0_5StateEEEENSt8__detail9_MakeUniqIT_E15__single_objectEDpOT0_.isra.0"
.LEHE24:
	mov	rax, QWORD PTR 40[rsp]
	lea	rcx, 48[rsp]
	mov	QWORD PTR 48[rsp], rax
.LEHB25:
	call	rbx
.LEHE25:
	mov	rcx, QWORD PTR 48[rsp]
	test	rcx, rcx
	je	.L216
	mov	rax, QWORD PTR [rcx]
	call	[QWORD PTR 8[rax]]
.L216:
	lea	rcx, 40[rsp]
	lea	r8, "_ZN12_GLOBAL__N_1L25BM_RegisterFile_EnumWriteERN9benchmark5StateE"[rip]
	mov	QWORD PTR 48[rsp], 0
	lea	rdx, .LC6[rip]
.LEHB26:
	call	"_ZSt11make_uniqueIN9benchmark8internal17FunctionBenchmarkEJRA26_KcPFvRNS0_5StateEEEENSt8__detail9_MakeUniqIT_E15__single_objectEDpOT0_.isra.0"
.LEHE26:
	mov	rax, QWORD PTR 40[rsp]
	lea	rcx, 48[rsp]
	mov	QWORD PTR 48[rsp], rax
.LEHB27:
	call	rbx
.LEHE27:
	mov	rcx, QWORD PTR 48[rsp]
	test	rcx, rcx
	je	.L217
	mov	rax, QWORD PTR [rcx]
	call	[QWORD PTR 8[rax]]
.L217:
	lea	rcx, 40[rsp]
	lea	r8, "_ZN12_GLOBAL__N_1L24BM_RegisterFile_EnumReadERN9benchmark5StateE"[rip]
	mov	QWORD PTR 48[rsp], 0
	lea	rdx, .LC7[rip]
.LEHB28:
	call	"_ZSt11make_uniqueIN9benchmark8internal17FunctionBenchmarkEJRA25_KcPFvRNS0_5StateEEEENSt8__detail9_MakeUniqIT_E15__single_objectEDpOT0_.isra.0"
.LEHE28:
	mov	rax, QWORD PTR 40[rsp]
	lea	rcx, 48[rsp]
	mov	QWORD PTR 48[rsp], rax
.LEHB29:
	call	rbx
.LEHE29:
	mov	rcx, QWORD PTR 48[rsp]
	test	rcx, rcx
	je	.L218
	mov	rax, QWORD PTR [rcx]
	call	[QWORD PTR 8[rax]]
.L218:
	lea	rcx, 40[rsp]
	lea	r8, "_ZN12_GLOBAL__N_1L25BM_RegisterFile_EnumWriteERN9benchmark5StateE"[rip]
	mov	QWORD PTR 48[rsp], 0
	lea	rdx, .LC6[rip]
.LEHB30:
	call	"_ZSt11make_uniqueIN9benchmark8internal17FunctionBenchmarkEJRA26_KcPFvRNS0_5StateEEEENSt8__detail9_MakeUniqIT_E15__single_objectEDpOT0_.isra.0"
.LEHE30:
	mov	rax, QWORD PTR 40[rsp]
	lea	rcx, 48[rsp]
	mov	QWORD PTR 48[rsp], rax
.LEHB31:
	call	rbx
.LEHE31:
	mov	rcx, QWORD PTR 48[rsp]
	test	rcx, rcx
	je	.L219
	mov	rax, QWORD PTR [rcx]
	call	[QWORD PTR 8[rax]]
.L219:
	lea	rcx, 40[rsp]
	lea	r8, "_ZN12_GLOBAL__N_1L24BM_RegisterFile_EnumReadERN9benchmark5StateE"[rip]
	mov	QWORD PTR 48[rsp], 0
	lea	rdx, .LC7[rip]
.LEHB32:
	call	"_ZSt11make_uniqueIN9benchmark8internal17FunctionBenchmarkEJRA25_KcPFvRNS0_5StateEEEENSt8__detail9_MakeUniqIT_E15__single_objectEDpOT0_.isra.0"
.LEHE32:
	mov	rax, QWORD PTR 40[rsp]
	lea	rcx, 48[rsp]
	mov	QWORD PTR 48[rsp], rax
.LEHB33:
	call	rbx
.LEHE33:
	mov	rcx, QWORD PTR 48[rsp]
	test	rcx, rcx
	je	.L220
	mov	rax, QWORD PTR [rcx]
	call	[QWORD PTR 8[rax]]
.L220:
	mov	ecx, 312
	pxor	xmm6, xmm6
	mov	QWORD PTR 40[rsp], 0
.LEHB34:
	call	"_Znwy"
.LEHE34:
	mov	ecx, 28
	movups	XMMWORD PTR 48[rsp], xmm6
	mov	rsi, rax
	movups	XMMWORD PTR 64[rsp], xmm6
.LEHB35:
	call	"_Znwy"
.LEHE35:
	movdqu	xmm0, XMMWORD PTR .LC12[rip]
	mov	BYTE PTR 27[rax], 0
	lea	rdx, 48[rsp]
	mov	rcx, rsi
	mov	QWORD PTR 48[rsp], rax
	mov	r12, QWORD PTR __imp__ZN9benchmark9BenchmarkC2ERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE[rip]
	movups	XMMWORD PTR [rax], xmm0
	movdqu	xmm0, XMMWORD PTR .LC13[rip]
	mov	QWORD PTR 64[rsp], 27
	mov	QWORD PTR 56[rsp], 27
	movups	XMMWORD PTR 11[rax], xmm0
.LEHB36:
	call	r12
.LEHE36:
	mov	rdi, QWORD PTR __imp__ZTVN9benchmark8internal17FunctionBenchmarkE[rip]
	mov	rcx, QWORD PTR 48[rsp]
	lea	rbp, 64[rsp]
	lea	rax, 16[rdi]
	mov	QWORD PTR [rsi], rax
	lea	rax, "_ZN12_GLOBAL__N_1L27BM_RegisterFile_StringWriteERN9benchmark5StateE"[rip]
	mov	QWORD PTR 304[rsi], rax
	cmp	rcx, rbp
	je	.L221
	mov	rax, QWORD PTR 64[rsp]
	lea	rdx, 1[rax]
	call	"_ZdlPvy"
.L221:
	mov	QWORD PTR 40[rsp], rsi
	lea	rcx, 40[rsp]
.LEHB37:
	call	rbx
.LEHE37:
	mov	rcx, QWORD PTR 40[rsp]
	test	rcx, rcx
	je	.L226
	mov	rax, QWORD PTR [rcx]
	call	[QWORD PTR 8[rax]]
.L226:
	lea	rcx, 40[rsp]
	lea	r8, "_ZN12_GLOBAL__N_1L26BM_RegisterFile_StringReadERN9benchmark5StateE"[rip]
	mov	QWORD PTR 48[rsp], 0
	lea	rdx, .LC8[rip]
.LEHB38:
	call	"_ZSt11make_uniqueIN9benchmark8internal17FunctionBenchmarkEJRA27_KcPFvRNS0_5StateEEEENSt8__detail9_MakeUniqIT_E15__single_objectEDpOT0_.isra.0"
.LEHE38:
	mov	rax, QWORD PTR 40[rsp]
	lea	rcx, 48[rsp]
	mov	QWORD PTR 48[rsp], rax
.LEHB39:
	call	rbx
.LEHE39:
	mov	rcx, QWORD PTR 48[rsp]
	test	rcx, rcx
	je	.L227
	mov	rax, QWORD PTR [rcx]
	call	[QWORD PTR 8[rax]]
.L227:
	lea	rcx, 40[rsp]
	lea	r8, "_ZN12_GLOBAL__N_1L26BM_RegisterFile_ArithmeticERN9benchmark5StateE"[rip]
	mov	QWORD PTR 48[rsp], 0
	lea	rdx, .LC9[rip]
.LEHB40:
	call	"_ZSt11make_uniqueIN9benchmark8internal17FunctionBenchmarkEJRA27_KcPFvRNS0_5StateEEEENSt8__detail9_MakeUniqIT_E15__single_objectEDpOT0_.isra.0"
.LEHE40:
	mov	rax, QWORD PTR 40[rsp]
	lea	rcx, 48[rsp]
	mov	QWORD PTR 48[rsp], rax
.LEHB41:
	call	rbx
.LEHE41:
	mov	rcx, QWORD PTR 48[rsp]
	test	rcx, rcx
	je	.L228
	mov	rax, QWORD PTR [rcx]
	call	[QWORD PTR 8[rax]]
.L228:
	mov	QWORD PTR 40[rsp], 0
	mov	ecx, 312
.LEHB42:
	call	"_Znwy"
.LEHE42:
	mov	ecx, 22
	movups	XMMWORD PTR 48[rsp], xmm6
	mov	rsi, rax
	movups	XMMWORD PTR 64[rsp], xmm6
.LEHB43:
	call	"_Znwy"
.LEHE43:
	movdqu	xmm0, XMMWORD PTR .LC12[rip]
	mov	BYTE PTR 21[rax], 0
	mov	rcx, rsi
	movabs	rdx, 8387236760058291564
	mov	QWORD PTR 48[rsp], rax
	movups	XMMWORD PTR [rax], xmm0
	mov	QWORD PTR 64[rsp], 21
	mov	QWORD PTR 13[rax], rdx
	lea	rdx, 48[rsp]
	mov	QWORD PTR 56[rsp], 21
.LEHB44:
	call	r12
.LEHE44:
	mov	rcx, QWORD PTR 48[rsp]
	add	rdi, 16
	lea	rax, "_ZN12_GLOBAL__N_1L21BM_RegisterFile_ResetERN9benchmark5StateE"[rip]
	mov	QWORD PTR [rsi], rdi
	mov	QWORD PTR 304[rsi], rax
	cmp	rcx, rbp
	je	.L229
	mov	rax, QWORD PTR 64[rsp]
	lea	rdx, 1[rax]
	call	"_ZdlPvy"
.L229:
	mov	QWORD PTR 40[rsp], rsi
	lea	rcx, 40[rsp]
.LEHB45:
	call	rbx
.LEHE45:
	mov	rcx, QWORD PTR 40[rsp]
	test	rcx, rcx
	je	.L234
	mov	rax, QWORD PTR [rcx]
	call	[QWORD PTR 8[rax]]
.L234:
	lea	rcx, 40[rsp]
	lea	r8, "_ZN12_GLOBAL__N_1L17BM_RawArray_WriteERN9benchmark5StateE"[rip]
	mov	QWORD PTR 48[rsp], 0
	lea	rdx, .LC10[rip]
.LEHB46:
	call	"_ZSt11make_uniqueIN9benchmark8internal17FunctionBenchmarkEJRA18_KcPFvRNS0_5StateEEEENSt8__detail9_MakeUniqIT_E15__single_objectEDpOT0_.isra.0"
.LEHE46:
	mov	rax, QWORD PTR 40[rsp]
	lea	rcx, 48[rsp]
	mov	QWORD PTR 48[rsp], rax
.LEHB47:
	call	rbx
.LEHE47:
	mov	rcx, QWORD PTR 48[rsp]
	test	rcx, rcx
	je	.L235
	mov	rax, QWORD PTR [rcx]
	call	[QWORD PTR 8[rax]]
.L235:
	lea	rcx, 40[rsp]
	lea	r8, "_ZN12_GLOBAL__N_1L16BM_RawArray_ReadERN9benchmark5StateE"[rip]
	mov	QWORD PTR 48[rsp], 0
	lea	rdx, .LC11[rip]
.LEHB48:
	call	"_ZSt11make_uniqueIN9benchmark8internal17FunctionBenchmarkEJRA17_KcPFvRNS0_5StateEEEENSt8__detail9_MakeUniqIT_E15__single_objectEDpOT0_.isra.0"
.LEHE48:
	mov	rax, QWORD PTR 40[rsp]
	lea	rcx, 48[rsp]
	mov	QWORD PTR 48[rsp], rax
.LEHB49:
	call	rbx
.LEHE49:
	mov	rcx, QWORD PTR 48[rsp]
	test	rcx, rcx
	je	.L213
	mov	rax, QWORD PTR [rcx]
	call	[QWORD PTR 8[rax]]
	nop
.L213:
	movups	xmm6, XMMWORD PTR 80[rsp]
	add	rsp, 96
	pop	rbx
	pop	rsi
	pop	rdi
	pop	rbp
	pop	r12
	ret
.L263:
.L364:
	mov	rcx, QWORD PTR 48[rsp]
	mov	rbx, rax
	test	rcx, rcx
	je	.L262
.L362:
	mov	rax, QWORD PTR [rcx]
	call	[QWORD PTR 8[rax]]
.L262:
	mov	rcx, rbx
.LEHB50:
	call	"_Unwind_Resume"
.L275:
	jmp	.L364
.L277:
	mov	rcx, QWORD PTR 48[rsp]
	mov	rbx, rax
	lea	rax, 64[rsp]
	cmp	rcx, rax
	je	.L233
.L361:
	mov	rax, QWORD PTR 64[rsp]
	lea	rdx, 1[rax]
	call	"_ZdlPvy"
.L233:
	mov	rcx, rsi
	mov	edx, 312
	call	"_ZdlPvy"
	mov	rcx, rbx
	call	"_Unwind_Resume"
.LEHE50:
.L276:
.L360:
	mov	rbx, rax
	jmp	.L233
.L278:
	jmp	.L360
.L272:
	jmp	.L364
.L271:
	jmp	.L364
.L270:
.L369:
	mov	rcx, QWORD PTR 40[rsp]
	mov	rbx, rax
	test	rcx, rcx
	jne	.L362
	jmp	.L262
.L273:
	jmp	.L369
.L274:
	jmp	.L364
.L279:
	mov	rcx, QWORD PTR 48[rsp]
	mov	rbx, rax
	cmp	rcx, rbp
	jne	.L361
	jmp	.L233
.L267:
	jmp	.L364
.L266:
	jmp	.L364
.L269:
	jmp	.L364
.L268:
	jmp	.L364
.L265:
	jmp	.L364
.L264:
	jmp	.L364
	.seh_handler	"__gxx_personality_seh0", @unwind, @except
	.seh_handlerdata
.LLSDA9654:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSE9654-.LLSDACSB9654
.LLSDACSB9654:
	.uleb128 .LEHB20-.LFB9654
	.uleb128 .LEHE20-.LEHB20
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB21-.LFB9654
	.uleb128 .LEHE21-.LEHB21
	.uleb128 .L263-.LFB9654
	.uleb128 0
	.uleb128 .LEHB22-.LFB9654
	.uleb128 .LEHE22-.LEHB22
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB23-.LFB9654
	.uleb128 .LEHE23-.LEHB23
	.uleb128 .L264-.LFB9654
	.uleb128 0
	.uleb128 .LEHB24-.LFB9654
	.uleb128 .LEHE24-.LEHB24
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB25-.LFB9654
	.uleb128 .LEHE25-.LEHB25
	.uleb128 .L265-.LFB9654
	.uleb128 0
	.uleb128 .LEHB26-.LFB9654
	.uleb128 .LEHE26-.LEHB26
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB27-.LFB9654
	.uleb128 .LEHE27-.LEHB27
	.uleb128 .L266-.LFB9654
	.uleb128 0
	.uleb128 .LEHB28-.LFB9654
	.uleb128 .LEHE28-.LEHB28
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB29-.LFB9654
	.uleb128 .LEHE29-.LEHB29
	.uleb128 .L267-.LFB9654
	.uleb128 0
	.uleb128 .LEHB30-.LFB9654
	.uleb128 .LEHE30-.LEHB30
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB31-.LFB9654
	.uleb128 .LEHE31-.LEHB31
	.uleb128 .L268-.LFB9654
	.uleb128 0
	.uleb128 .LEHB32-.LFB9654
	.uleb128 .LEHE32-.LEHB32
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB33-.LFB9654
	.uleb128 .LEHE33-.LEHB33
	.uleb128 .L269-.LFB9654
	.uleb128 0
	.uleb128 .LEHB34-.LFB9654
	.uleb128 .LEHE34-.LEHB34
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB35-.LFB9654
	.uleb128 .LEHE35-.LEHB35
	.uleb128 .L276-.LFB9654
	.uleb128 0
	.uleb128 .LEHB36-.LFB9654
	.uleb128 .LEHE36-.LEHB36
	.uleb128 .L277-.LFB9654
	.uleb128 0
	.uleb128 .LEHB37-.LFB9654
	.uleb128 .LEHE37-.LEHB37
	.uleb128 .L270-.LFB9654
	.uleb128 0
	.uleb128 .LEHB38-.LFB9654
	.uleb128 .LEHE38-.LEHB38
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB39-.LFB9654
	.uleb128 .LEHE39-.LEHB39
	.uleb128 .L271-.LFB9654
	.uleb128 0
	.uleb128 .LEHB40-.LFB9654
	.uleb128 .LEHE40-.LEHB40
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB41-.LFB9654
	.uleb128 .LEHE41-.LEHB41
	.uleb128 .L272-.LFB9654
	.uleb128 0
	.uleb128 .LEHB42-.LFB9654
	.uleb128 .LEHE42-.LEHB42
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB43-.LFB9654
	.uleb128 .LEHE43-.LEHB43
	.uleb128 .L278-.LFB9654
	.uleb128 0
	.uleb128 .LEHB44-.LFB9654
	.uleb128 .LEHE44-.LEHB44
	.uleb128 .L279-.LFB9654
	.uleb128 0
	.uleb128 .LEHB45-.LFB9654
	.uleb128 .LEHE45-.LEHB45
	.uleb128 .L273-.LFB9654
	.uleb128 0
	.uleb128 .LEHB46-.LFB9654
	.uleb128 .LEHE46-.LEHB46
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB47-.LFB9654
	.uleb128 .LEHE47-.LEHB47
	.uleb128 .L274-.LFB9654
	.uleb128 0
	.uleb128 .LEHB48-.LFB9654
	.uleb128 .LEHE48-.LEHB48
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB49-.LFB9654
	.uleb128 .LEHE49-.LEHB49
	.uleb128 .L275-.LFB9654
	.uleb128 0
	.uleb128 .LEHB50-.LFB9654
	.uleb128 .LEHE50-.LEHB50
	.uleb128 0
	.uleb128 0
.LLSDACSE9654:
	.section	.text.startup,"x"
	.seh_endproc
	.text
	.p2align 4
	.def	"_ZN12_GLOBAL__N_1L26BM_RegisterFile_StringReadERN9benchmark5StateE";	.scl	3;	.type	32;	.endef
	.seh_proc	"_ZN12_GLOBAL__N_1L26BM_RegisterFile_StringReadERN9benchmark5StateE"
"_ZN12_GLOBAL__N_1L26BM_RegisterFile_StringReadERN9benchmark5StateE":
.LFB7862:
	push	rdi
	.seh_pushreg	rdi
	push	rsi
	.seh_pushreg	rsi
	push	rbx
	.seh_pushreg	rbx
	sub	rsp, 48
	.seh_stackalloc	48
	.seh_endprologue
	mov	eax, DWORD PTR 28[rcx]
	mov	rsi, rcx
	test	eax, eax
	jne	.L387
	mov	rbx, QWORD PTR 16[rcx]
	call	[QWORD PTR __imp__ZN9benchmark5State16StartKeepRunningEv[rip]]
	test	rbx, rbx
	je	.L378
	jle	.L388
	.p2align 4
	.p2align 3
.L379:
	mov	QWORD PTR 32[rsp], 123456
	mov	QWORD PTR 40[rsp], 0
	sub	rbx, 1
	jne	.L379
.L378:
	mov	rcx, rsi
	add	rsp, 48
	pop	rbx
	pop	rsi
	pop	rdi
	rex.W jmp	[QWORD PTR __imp__ZN9benchmark5State17FinishKeepRunningEv[rip]]
	.p2align 4,,10
	.p2align 3
.L388:
	mov	rdi, QWORD PTR __imp__assert[rip]
	.p2align 4
	.p2align 3
.L380:
	mov	QWORD PTR 32[rsp], 123456
	mov	QWORD PTR 40[rsp], 0
	mov	r8d, 257
	lea	rdx, .LC1[rip]
	lea	rcx, .LC2[rip]
	call	rdi
	sub	rbx, 1
	jne	.L380
	mov	rcx, rsi
	add	rsp, 48
	pop	rbx
	pop	rsi
	pop	rdi
	rex.W jmp	[QWORD PTR __imp__ZN9benchmark5State17FinishKeepRunningEv[rip]]
	.p2align 4,,10
	.p2align 3
.L387:
	call	[QWORD PTR __imp__ZN9benchmark5State16StartKeepRunningEv[rip]]
	mov	rcx, rsi
	add	rsp, 48
	pop	rbx
	pop	rsi
	pop	rdi
	rex.W jmp	[QWORD PTR __imp__ZN9benchmark5State17FinishKeepRunningEv[rip]]
	.seh_endproc
	.p2align 4
	.def	"_ZN12_GLOBAL__N_1L26BM_RegisterFile_ArithmeticERN9benchmark5StateE";	.scl	3;	.type	32;	.endef
	.seh_proc	"_ZN12_GLOBAL__N_1L26BM_RegisterFile_ArithmeticERN9benchmark5StateE"
"_ZN12_GLOBAL__N_1L26BM_RegisterFile_ArithmeticERN9benchmark5StateE":
.LFB7863:
	push	rdi
	.seh_pushreg	rdi
	push	rsi
	.seh_pushreg	rsi
	push	rbx
	.seh_pushreg	rbx
	sub	rsp, 48
	.seh_stackalloc	48
	.seh_endprologue
	mov	eax, DWORD PTR 28[rcx]
	mov	rsi, rcx
	test	eax, eax
	jne	.L400
	mov	rbx, QWORD PTR 16[rcx]
	call	[QWORD PTR __imp__ZN9benchmark5State16StartKeepRunningEv[rip]]
	test	rbx, rbx
	je	.L391
	jle	.L401
	.p2align 4
	.p2align 3
.L392:
	mov	QWORD PTR 32[rsp], 300
	mov	QWORD PTR 40[rsp], 0
	sub	rbx, 1
	jne	.L392
.L391:
	mov	rcx, rsi
	add	rsp, 48
	pop	rbx
	pop	rsi
	pop	rdi
	rex.W jmp	[QWORD PTR __imp__ZN9benchmark5State17FinishKeepRunningEv[rip]]
	.p2align 4,,10
	.p2align 3
.L401:
	mov	rdi, QWORD PTR __imp__assert[rip]
	.p2align 4
	.p2align 3
.L393:
	mov	QWORD PTR 32[rsp], 300
	mov	QWORD PTR 40[rsp], 0
	mov	r8d, 257
	lea	rdx, .LC1[rip]
	lea	rcx, .LC2[rip]
	call	rdi
	sub	rbx, 1
	jne	.L393
	mov	rcx, rsi
	add	rsp, 48
	pop	rbx
	pop	rsi
	pop	rdi
	rex.W jmp	[QWORD PTR __imp__ZN9benchmark5State17FinishKeepRunningEv[rip]]
	.p2align 4,,10
	.p2align 3
.L400:
	call	[QWORD PTR __imp__ZN9benchmark5State16StartKeepRunningEv[rip]]
	mov	rcx, rsi
	add	rsp, 48
	pop	rbx
	pop	rsi
	pop	rdi
	rex.W jmp	[QWORD PTR __imp__ZN9benchmark5State17FinishKeepRunningEv[rip]]
	.seh_endproc
	.section	.text.startup,"x"
	.p2align 4
	.def	"_GLOBAL__sub_I_RegisterFileBenchmark.cpp";	.scl	3;	.type	32;	.endef
	.seh_proc	"_GLOBAL__sub_I_RegisterFileBenchmark.cpp"
"_GLOBAL__sub_I_RegisterFileBenchmark.cpp":
.LFB10768:
	.seh_endprologue
	jmp	"_Z41__static_initialization_and_destruction_0v"
	.seh_endproc
	.section	.ctors,"w"
	.align 8
	.quad	"_GLOBAL__sub_I_RegisterFileBenchmark.cpp"
	.section .rdata,"dr"
	.align 16
.LC12:
	.quad	8316291871787666754
	.quad	6874019605660198260
	.align 16
.LC13:
	.quad	8247308486810822982
	.quad	7310584034942742121
	.def	"__gxx_personality_seh0";	.scl	2;	.type	32;	.endef
	.ident	"GCC: (Rev6, Built by MSYS2 project) 16.2.0"
	.def	"memcpy";	.scl	2;	.type	32;	.endef
	.def	"_Znwy";	.scl	2;	.type	32;	.endef
	.def	"strlen";	.scl	2;	.type	32;	.endef
	.def	"_ZdlPvy";	.scl	2;	.type	32;	.endef
	.def	"_Unwind_Resume";	.scl	2;	.type	32;	.endef
