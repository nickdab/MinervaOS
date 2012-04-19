#include "multiboot.h"

#define CHECK_FLAG(flags,bit)	((flags) & (1 << (bit)))

#define COLUMNS		80
#define LINES		24
#define ATTRIBUTE	7

#define VIDEO		0xB8000

void kmain(unsigned long magic, unsigned long addr);
static void cls(void);
static void itoa(char *buf, int base, int d);
static void putchar(int c);
void printf(const char *format, ...);

static int xpos;
static int ypos;
static volatile unsigned char *video;

void kmain(unsigned long magic, unsigned long addr)
{
	multiboot_t_info *mbi = (multiboot_t_info*)addr;

	if (magic != 0x2BADB002)	//something is wrong
	{
		//printf("Bad Magic number from bootloader.\n");
		return;
	}
	
	cls();
	
	if (CHECK_FLAG(mbi->flags,11))
	{
		putchar('t');
	}
	else
	{
		putchar('f');
	}
	

	return ;
}

static void putchar(int c)
{
	*(video + (xpos+ypos * COLUMNS) * 2) = c & 0xFF;
	*(video + (xpos+ypos * COLUMNS) * 2 + 1) = ATTRIBUTE;
	
	xpos++;
}

static void cls(void)
{
	int i;

	video = (unsigned char *) VIDEO;
	
	for (i=0;i<COLUMNS*LINES*2;i++)
	{
		*(video+i)=0;
	}
	
	xpos = 0;
	ypos = 0;
}
