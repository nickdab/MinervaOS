#include "Multiboot.h"

int kmain()
{
	asm("movl $0x2BADB002, %eax\n\t");
	return 0;
}
