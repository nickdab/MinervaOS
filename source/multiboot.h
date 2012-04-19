#ifndef MULTIBOOT_H
#define MULTIBOOT_H

/*These are all the options available for MULTIBOOT_FLAGS*/

#define FLAGS_PAGE_ALIGNED	0x00000001	// sets bit [0], meaning all boot modules have to be aligned to page (4K) boundries
#define FLAGS_MEM_INFO		0x00000002	// sets bit [1], meaning info on memory will be provided through at least the mem_* structure
#define FLAGS_VMT_INFO		0x00000004	// sets bit [2], meaning we will provide info about the video mode we want
#define FLAGS_ADDR_TABLE	0x00010000	// sets bit [16], meaning that we are provided address information (only if the OS isn't in ELF format)

#define MULTIBOOT_MAGIC		0x1BADB002
#define MULTIBOOT_FLAGS 	(FLAGS_PAGE_ALIGNED)	// sets bit [2] -- we have to provide info about Video Mode Table

#ifndef ASM_FILE

typedef char			multiboot_t_u8;
typedef unsigned short		multiboot_t_u16;
typedef unsigned long		multiboot_t_u32;
typedef unsigned long long	multiboot_t_u64;

struct multiboot_mods
{
	multiboot_t_u32		mod_start;
	multiboot_t_u32		mod_end;

	multiboot_t_u32		string;
	
	multiboot_t_u32		reserved;
};
typedef multiboot_mods 		multiboot_t_mods;

struct multiboot_aout_info
{
	multiboot_t_u32		tabsize;
	multiboot_t_u32		strsize;
	multiboot_t_u32		addr;
	multiboot_t_u32		reserved;
};
typedef multiboot_aout_info	multiboot_t_aout_info;

struct multiboot_elf_info
{
	multiboot_t_u32		num;
	multiboot_t_u32		size;
	multiboot_t_u32		addr;
	multiboot_t_u32		shndx;
};
typedef multiboot_elf_info	multiboot_t_elf_info;


struct multiboot_info
{
	//flags set by bootloader
	multiboot_t_u32		flags;
	
	//bit[0] of flags set --> these tell how much memory the computer has
	multiboot_t_u32		mem_lower;
	multiboot_t_u32		mem_upper;

	//bit[1] of flags set --> BIOS disk device from int 0x13
	multiboot_t_u32		boot_device;
	
	//bit[2] of flags set --> command-line c-style string address
	multiboot_t_u32		cmdline;

	//bit[3] of flags set --> we might have modules 
	multiboot_t_u32		mods_count;
	multiboot_t_u32		mods_addr;

	//bit[4] (a.out format) or bit[5] (ELF) set (mutually exclusive) -->
	//respective file structures loaded for a.out or ELF
	union
	{
		multiboot_t_aout_info 	aout_sym;
		multiboot_t_elf_info	elf_sec;
	}u;	
	
	//bit[6] of flags set --> memory map available from BIOS
	multiboot_t_u32		mmap_length;
	multiboot_t_u32		mmap_addr;

	//bit[7] of flags set --> information about drives available
	multiboot_t_u32		drives_length;
	multiboot_t_u32		drives_addr;

	//bit[8] of flags set --> configuration table address
	multiboot_t_u32		config_table;
	
	//bit[9] of flags set --> boot loader C-String address
	multiboot_t_u32		bootloader_name;

	//bit[10] of flags set --> apm_table address
	multiboot_t_u32		apm_table;

	//bit[11] of flags set --> graphics table available
	multiboot_t_u32		vbe_control_info;
	multiboot_t_u32		vbe_mode_info;
	multiboot_t_u16		vbe_mode;
	multiboot_t_u16		vbe_interface_seg;
	multiboot_t_u16		vbe_interfcace_off;
	multiboot_t_u16		vbe_interface_len;
};
typedef multiboot_info		multiboot_t_info;


#endif
