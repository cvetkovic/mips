.syntax unified
.cpu cortex-m3
.fpu softvfp
.thumb

.extern _main_stack_pointer
.extern main
.extern _lma_data_start
.extern _vma_data_start
.extern _vma_data_end
.extern timer_handler
.extern ext_handler

.weak timer_handler
.thumb_set timer_handler, default_handler
.weak ext_handler
.thumb_set ext_handler, default_handler

.section .vector_table, "a"
.word _main_stack_pointer
.word reset_handler
.rept 39
	.word default_handler
.endr
.word timer_handler
.rept 14
	.word default_handler
.endr
.word ext_handler
.rept 27
	.word default_handler
.endr

.section .text.default_handler
.type default_handler, %function
default_handler:
	b default_handler

.section .text.reset_handler:
.type reset_handler, %function
reset_handler:
	ldr r0, =_lma_data_start
	ldr r1, =_vma_data_start
	ldr r2, =_vma_data_end
	cmp r1, r2
	beq program
	
copy_loop:
	ldr r3, [r0], #4
	str r3, [r1], #4
	cmp r1, r2
	blo copy_loop
	
program:
	bl main
	
infinite_loop:
	b infinite_loop
	
	