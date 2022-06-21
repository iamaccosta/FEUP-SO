	.file	"char_array.c"
	.text
	.section	.rodata
.LC0:
	.string	"%c <--> %c\n"
.LC1:
	.string	"%d <--> %d\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$64, %rsp
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movabsq	$8022916924116329800, %rax
	movq	%rax, -20(%rbp)
	movl	$6581362, -12(%rbp)
	movl	$0, -52(%rbp)
	jmp	.L2
.L3:
	movl	-52(%rbp), %eax
	cltq
	leaq	-20(%rbp), %rdx
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %edx
	movl	-52(%rbp), %eax
	cltq
	movzbl	-20(%rbp,%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %esi
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	addl	$1, -52(%rbp)
.L2:
	movl	-52(%rbp), %eax
	cmpl	$11, %eax
	jbe	.L3
	movl	$2, -48(%rbp)
	movl	$3, -44(%rbp)
	movl	$5, -40(%rbp)
	movl	$7, -36(%rbp)
	movl	$11, -32(%rbp)
	movl	$0, -52(%rbp)
	jmp	.L4
.L5:
	movl	-52(%rbp), %eax
	cltq
	leaq	0(,%rax,4), %rdx
	leaq	-48(%rbp), %rax
	addq	%rdx, %rax
	movl	(%rax), %edx
	movl	-52(%rbp), %eax
	cltq
	movl	-48(%rbp,%rax,4), %eax
	movl	%eax, %esi
	leaq	.LC1(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	addl	$1, -52(%rbp)
.L4:
	movl	-52(%rbp), %eax
	cmpl	$4, %eax
	jbe	.L5
	movl	$0, %eax
	movq	-8(%rbp), %rcx
	xorq	%fs:40, %rcx
	je	.L7
	call	__stack_chk_fail@PLT
.L7:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 9.4.0-1ubuntu1~20.04) 9.4.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	 1f - 0f
	.long	 4f - 1f
	.long	 5
0:
	.string	 "GNU"
1:
	.align 8
	.long	 0xc0000002
	.long	 3f - 2f
2:
	.long	 0x3
3:
	.align 8
4:
