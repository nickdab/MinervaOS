#include "multiboot.h"

#define CHECK_FLAG(flags,bit)	((flags) & (1 << (bit)))

#define COLUMNS		80
#define LINES		24
#define ATTRIBUTE	0x0B

#define VIDEO		0xB8000

void kmain(unsigned long magic, unsigned long addr);
static void cls(void);
static void putchar(int c);
static void itoa(int value, char *buf, int base);
void printf(const char *format, ...);
void newline();

static int xpos;
static int ypos;
static volatile unsigned char *video;

void kmain(unsigned long magic, unsigned long addr)
{
	cls();
	multiboot_t_info *mbi = (multiboot_t_info*)addr;

	if (magic != 0x2BADB002)	//something is wrong
	{
		printf("Bad Magic number from bootloader.\n");
		return;
	}
	
	printf("%x\n%x\n%x\n%x",100, 16, 437, 99812321 );
	

	return ;
}

void newline()
{
	xpos=0;
	ypos++;

	if (ypos >= LINES)
	{
		for (int i = 0; i < LINES-1; i++)
		{
			*(video + (xpos+ypos * COLUMNS) * 2) = *(video + (xpos+(ypos+1) * COLUMNS) *2);
		}
		ypos--;
	}
}
		 

static void putchar(int c)
{
	if (c == '\n' || c== '\t')
	{
		newline();
	}
	else
	{
		*(video + (xpos+ypos * COLUMNS) *2) = c & 0xFF;
		*(video + (xpos+ypos * COLUMNS) * 2 + 1) = ATTRIBUTE;
	
		xpos++;
	
		if (xpos >= COLUMNS)
		{
			newline();
		}
	}
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

void printf(const char *format, ...) 
{
	char **arg = (char **) &format;
	int c;
	char buf[20];
	
	*arg++;

	while ((c=*format++)!= 0)
	{
		if (c != '%')
		{
			putchar(c);
		}
		else
		{
			char *p;
	
			c = *format++;
			
			switch(c)
			{
				case 'd':
				case 'u':
				case 'x':
					itoa(*((int *)arg++),buf,'x');
					p=buf;
					goto string;
					break;
										
				case 's':
					p = *arg++;

					if (!p)
					{
						p="(null)";
					}
				
				string:
					while (*p)
						putchar(*p++);
					break;
			}
		}
	}
}

static void itoa(int value,char *buf, int base)
{
	char *str = buf;
	int divisor = 10;
	
	if (base == 'x')
	{
		divisor = 16;
	}
	
	do
	{
		int remainder = value % divisor;

		if (remainder <10)
		{
			*str++ = '0' + remainder;
		}
		else
		{
			*str++ = ('a' + (remainder-10));
		}
	}while (value /= divisor);

	*str = 0;
	
	char *p1, *p2;
	p1 = buf;
	p2= str-1;
	
	//now we have to reverse the string so it comes out right
	while (p1 < p2)
	{
		char tmp = *p1;
		*p1=*p2;
		*p2=tmp;
		p1++;
		p2--;
	} 
	
	if (base == 'x')
	{
		for (str; str >= buf; str--)
		{
			*(str+2) = *str;
			
		}
		
		*buf = '0';
		*(buf+1) = 'x';
	}
				
	
}	
