#define ASM_FILE	1
#include "multiboot.h"


#define STACK_SIZE	0x4000

.text

.globl start, _start

start:
_start:
	jmp multiboot_entry
	
	.align 4

multiboot_header:
	.long MULTIBOOT_MAGIC
	.long MULTIBOOT_FLAGS
	.long -(MULTIBOOT_MAGIC+MULTIBOOT_FLAGS)
	.long MULTIBOOT_HEADER_ADDR
	.long MULTIBOOT_LOAD_ADDR
	.long MULTIBOOT_LOAD_END_ADDR
	.long MULTIBOOT_BSS_END_ADDR
	.long MULTIBOOT_ENTRY_ADDR
	.long MULTIBOOT_GRAPHICS_MODE_TYPE
	.long MULTIBOOT_GRAPHICS_WIDTH
	.long MULTIBOOT_GRAPHICS_HEIGHT
	.long MULTIBOOT_GRAPHICS_DEPTH

multiboot_entry:
	movl $(stack+STACK_SIZE), %esp
	
	pushl $0
	popf
	
	pushl %ebx
	pushl %eax

	call kmain
	
	pushl $halt_message
	call printf

loop:
	hlt
	jmp loop

halt_message:
	.asciz "\nHalted."
	
	.comm stack, STACK_SIZE
