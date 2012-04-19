#include "multiboot.h"

void kmain(unsigned long magic, unsigned long addr);
static void cls(void);
static void itoa(char *buf, int base, int d);
static void putchar(int c);
void printf(const char *format, ...);

void kmain(unsigned long magic, unsigned long addr)
{
	multiboot_info_t *mbi;

	if (magic != 0x2BADB002)	//something is wrong
	{
		printf("Bad Magic number from bootloader.\n");
		return;
	}
	return ;
}
