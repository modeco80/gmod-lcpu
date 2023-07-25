# Simple bare-metal RISCV startup code.

.section .initial_jump
.global _start

.extern main

.align 4
_start:
	la sp, _sstack	# set up C stack
	addi sp,sp,-16  # ...
	sw ra,12(sp)    # ...
	jal ra, main	# jump to C code!
