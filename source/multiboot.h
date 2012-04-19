#ifndef MULTIBOOT_H
#define MULTIBOOT_H

#define MULTIBOOT_MAGIC		0x1BADB002
#define MULTIBOOT_FLAGS 	0x00000004	// sets bit [2] -- we have to provide info about Video Mode Table

//preferred graphics mode settings:
#define MULTIBOOT_VMODE		0x00000001	// EGA-standard text mode

#define MULTIBOOT_VWIDTH	0		// we don't care
#define MULTIBOOT_VHEIGHT 	0		// we don't care
#define MULTIBOOT_VDEPTH	0		// we don't care



#endif
