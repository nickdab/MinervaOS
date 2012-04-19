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

multiboot_entry:
	movl $(stack+STACK_SIZE), %esp
	
	pushl $0
	popf
	
	pushl %ebx
	pushl %eax

	call kmain
	
	pushl $halt_message
	/*call printf*/

loop:
	hlt
	jmp loop

halt_message:
	.asciz "Halted."
	
	.comm stack, STACK_SIZE
