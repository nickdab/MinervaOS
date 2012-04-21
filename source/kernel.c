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
	
	if (CHECK_FLAG(mbi->flags,0))
	{
		//mem_* fields are valid
		printf("LOWMEM: %iK, HIGHMEM: %iK\n",mbi->mem_lower,mbi->mem_upper);	
	}
	else
	{
		printf("No memory information present.\n");
	}
	
	if (CHECK_FLAG(mbi->flags,1))
	{
		//"boot device" field valid
		multiboot_t_u32 dev_num = mbi->boot_device;		

		dev_num = ((dev_num & 0xFF000000) >> 24);		//this zeros out all but the highest byte, which has the drive no. in it, then moves it to the lsb	

		printf("Device number is %x, ",dev_num);
		
		multiboot_t_u32 top_partition = mbi->boot_device;
		
		top_partition = ((top_partition & 0x00FF0000) >> 16);
		
		printf("Top-Level Partition number: %d",top_partition);
	
		multiboot_t_u32 sub_partition = mbi->boot_device;
		
		sub_partition = ((sub_partition & 0x0000FF00) >> 8);
		
		if (sub_partition != 0xFF)
		{
			printf(", sub partition: %x",sub_partition);
		}
		
		multiboot_t_u32 sub_sub_partition = mbi->boot_device;
		
		sub_sub_partition = ((sub_sub_partition & 0x000000FF));
		
		if (sub_sub_partition != 0xFF)
		{
			printf(", sub-sub partition: %x",sub_sub_partition);
		}

		printf("\n");
	}
	
	if (CHECK_FLAG(mbi->flags,2))
	{
		//cmdline field valid
		
		printf("cmdline: \"%s\"\n",mbi->cmdline);
	}
	else
	{
		printf("no cmdline field.\n");
	}
	
	if (CHECK_FLAG(mbi->flags,3))
	{
		//mods field valid
		printf("There are %i modules.\n",mbi->mods_count);
	}
	else
	{
		printf("Mods flag invalid");
	}
	
	if (CHECK_FLAG(mbi->flags,4))
	{
		//a.out file extensions valid
		printf("a.out tabsize: %x\n",mbi->u.aout_sym.tabsize);
	}
	else
	{
		printf("a.out field invalid.\n");
	}
	
	if (CHECK_FLAG(mbi->flags,5))
	{
		printf("ELF num: %i\n", mbi->u.elf_sec.num);
	}
	else
	{
		printf("ELF field invalid.\n");
	}
	
	if (CHECK_FLAG(mbi->flags,6))
	{
		//memory map information valid
		multiboot_t_mmap *mmap_info_p = (multiboot_t_mmap *)mbi->mmap_addr;
		printf("%i, %i",mbi->mmap_length, mmap_info_p->size);
	
		int i = 0;

		while ((unsigned long )mmap_info_p < (mbi->mmap_addr + mbi->mmap_length))
		{
			printf("Memory map %i:\n",i);

			printf("base address: 0x%x\n",(mmap_info_p->base_addr));	

			printf("length: 0x%x\n",(mmap_info_p->length));

			//note, there is a problem with these being 64 bit numbers, but i dont know what to do about it
			
			if (mmap_info_p->type == 1)
			{
				printf("Available\n\n");
			}
			else
			{
				printf("Not Available\n\n");
			}
			
			mmap_info_p =(multiboot_t_mmap *) ((unsigned long )mmap_info_p + mmap_info_p->size+sizeof(mmap_info_p->size));
			i++;
		}
	}

	if (CHECK_FLAG(mbi->flags,7))
	{
		//Drive info availale
	}
	
	
	
	return ;
}

void newline()
{
	xpos=0;
	ypos++;

	if (ypos >= LINES)
	{
		cls();
		ypos=0;
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
				case 'i':
					//goto case 'd'
				case 'd':
					itoa(*((int *)arg++),buf,'d');
					p=buf;
					goto string;
					break;
					
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
}	
