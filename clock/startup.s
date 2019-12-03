.syntax unified
.cpu cortex-m3
.fpu softvfp
.thumb

.extern _main_stack_pointer
.extern main
.extern _lma_data_start
.extern _vma_data_start
.extern _vma_data_end
.extern exti_1_handler
.extern timer_1_update_handler

.section .vector_table, "a"
.word _main_stack_pointer
.word reset_handler
.rept 21
	.word default_handler
.endr
.word exti_1_handler
.rept 17
	.word default_handler
.endr
.word timer_1_update_handler
.rept 42
	.word default_handler
.endr

.section .text.default_handler
.type default_handler, %function
default_handler:
	b default_handler
	
.section .text.reset_handler
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
	bne copy_loop
	
program:
	bl main
	
infinite_loop:
	b infinite_loop