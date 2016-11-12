	.text
	.file	"main-llvm.bc"
                                        # Start of file scope inline assembly
	.ident	"GCC: (Ubuntu 4.8.4-2ubuntu1~14.04.3) 4.8.4 LLVM: 3.5.2svn"


	.ident	"GCC: (Ubuntu 4.8.4-2ubuntu1~14.04.3) 4.8.4 LLVM: 3.5.2svn"


                                        # End of file scope inline assembly
	.globl	sum_
	.align	16, 0x90
	.type	sum_,@function
sum_:                                   # @sum_
	.cfi_startproc
# BB#0:                                 # %return
	pushq	%rbp
.Ltmp0:
	.cfi_def_cfa_offset 16
.Ltmp1:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
.Ltmp2:
	.cfi_def_cfa_register %rbp
	pushq	%rbx
	subq	$1944, %rsp             # imm = 0x798
.Ltmp3:
	.cfi_offset %rbx, -24
	movq	$.L.cst, -496(%rbp)
	movl	$3, -488(%rbp)
	movl	$128, -504(%rbp)
	movl	$6, -500(%rbp)
	leaq	-504(%rbp), %rbx
	movq	%rbx, %rdi
	callq	_gfortran_st_write
	movl	$.L.cst1, %esi
	movl	$18, %edx
	movq	%rbx, %rdi
	callq	_gfortran_transfer_character_write
	movq	%rbx, %rdi
	callq	_gfortran_st_write_done
	movq	$.L.cst, -976(%rbp)
	movl	$4, -968(%rbp)
	movl	$128, -984(%rbp)
	movl	$5, -980(%rbp)
	leaq	-984(%rbp), %rbx
	movq	%rbx, %rdi
	callq	_gfortran_st_read
	leaq	-16(%rbp), %rsi
	movl	$4, %edx
	movq	%rbx, %rdi
	callq	_gfortran_transfer_real
	movq	%rbx, %rdi
	callq	_gfortran_st_read_done
	movq	$.L.cst, -1456(%rbp)
	movl	$5, -1448(%rbp)
	movl	$128, -1464(%rbp)
	movl	$5, -1460(%rbp)
	leaq	-1464(%rbp), %rbx
	movq	%rbx, %rdi
	callq	_gfortran_st_read
	leaq	-20(%rbp), %rsi
	movl	$4, %edx
	movq	%rbx, %rdi
	callq	_gfortran_transfer_real
	movq	%rbx, %rdi
	callq	_gfortran_st_read_done
	movss	-16(%rbp), %xmm0
	addss	-20(%rbp), %xmm0
	movss	%xmm0, -12(%rbp)
	movq	$.L.cst, -1936(%rbp)
	movl	$7, -1928(%rbp)
	movl	$128, -1944(%rbp)
	movl	$6, -1940(%rbp)
	leaq	-1944(%rbp), %rbx
	movq	%rbx, %rdi
	callq	_gfortran_st_write
	movl	$.L.cst2, %esi
	movl	$15, %edx
	movq	%rbx, %rdi
	callq	_gfortran_transfer_character_write
	leaq	-12(%rbp), %rsi
	movl	$4, %edx
	movq	%rbx, %rdi
	callq	_gfortran_transfer_real_write
	movq	%rbx, %rdi
	callq	_gfortran_st_write_done
	addq	$1944, %rsp             # imm = 0x798
	popq	%rbx
	popq	%rbp
	retq
.Ltmp4:
	.size	sum_, .Ltmp4-sum_
	.cfi_endproc

	.globl	main
	.align	16, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# BB#0:                                 # %entry
	pushq	%rbp
.Ltmp5:
	.cfi_def_cfa_offset 16
.Ltmp6:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
.Ltmp7:
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movl	%edi, -4(%rbp)
	movq	%rsi, -16(%rbp)
	movl	-4(%rbp), %edi
	callq	_gfortran_set_args
	movl	$7, %edi
	movl	$options.1.1882, %esi
	callq	_gfortran_set_options
	callq	MAIN__
	movl	$0, -20(%rbp)
	movl	-20(%rbp), %eax
	addq	$32, %rsp
	popq	%rbp
	retq
.Ltmp8:
	.size	main, .Ltmp8-main
	.cfi_endproc

	.align	16, 0x90
	.type	MAIN__,@function
MAIN__:                                 # @MAIN__
	.cfi_startproc
# BB#0:                                 # %return
	pushq	%rbp
.Ltmp9:
	.cfi_def_cfa_offset 16
.Ltmp10:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
.Ltmp11:
	.cfi_def_cfa_register %rbp
	pushq	%rbx
	subq	$488, %rsp              # imm = 0x1E8
.Ltmp12:
	.cfi_offset %rbx, -24
	movq	$.L.cst3, -480(%rbp)
	movl	$2, -472(%rbp)
	movl	$128, -488(%rbp)
	movl	$6, -484(%rbp)
	leaq	-488(%rbp), %rbx
	movq	%rbx, %rdi
	callq	_gfortran_st_write
	movl	$.L.cst14, %esi
	movl	$11, %edx
	movq	%rbx, %rdi
	callq	_gfortran_transfer_character_write
	movq	%rbx, %rdi
	callq	_gfortran_st_write_done
	callq	sum_
	addq	$488, %rsp              # imm = 0x1E8
	popq	%rbx
	popq	%rbp
	retq
.Ltmp13:
	.size	MAIN__, .Ltmp13-MAIN__
	.cfi_endproc

	.type	.L.cst,@object          # @.cst
	.section	.rodata,"a",@progbits
	.align	8
.L.cst:
	.asciz	"sum.f90"
	.size	.L.cst, 8

	.type	.L.cst1,@object         # @.cst1
	.align	8
.L.cst1:
	.ascii	"Enter Two Numbers:"
	.size	.L.cst1, 18

	.type	.L.cst2,@object         # @.cst2
	.align	8
.L.cst2:
	.ascii	"The total is : "
	.size	.L.cst2, 15

	.type	.L.cst3,@object         # @.cst3
	.align	8
.L.cst3:
	.asciz	"hello.f90"
	.size	.L.cst3, 10

	.type	.L.cst14,@object        # @.cst14
	.align	8
.L.cst14:
	.ascii	"Hello World"
	.size	.L.cst14, 11

	.type	options.1.1882,@object  # @options.1.1882
	.align	16
options.1.1882:
	.long	68                      # 0x44
	.long	1023                    # 0x3ff
	.long	0                       # 0x0
	.long	0                       # 0x0
	.long	1                       # 0x1
	.long	1                       # 0x1
	.long	0                       # 0x0
	.size	options.1.1882, 28


	.section	".note.GNU-stack","",@progbits
