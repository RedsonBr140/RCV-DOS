.section .rodata
message:
.asciz "Hello! This is RCV-DOS.\nGotta run C code now, bro.\n"

.equ MESSAGE_LENGTH, . - message - 1

.equ SBI_EXT_BASE, 0x10
.equ SBI_EXT_DBCN, 0x4442434E

.equ SBI_BASE_GET_SPEC_VERSION, 0

.section .text.entry
.globl _start
.type _start, @function
.extern kernel_main

_start:
	la sp, stack_top

	li a7, SBI_EXT_DBCN
	li a6, 0
	la a1, message
	li a0, MESSAGE_LENGTH
	li a2, 0

	ecall

	call kernel_main

halt:
    j halt

.size _start, .-_start

.section .bss.stack, "aw",@nobits
.align 4

stack_bottom:
	.space 16384
stack_top:
