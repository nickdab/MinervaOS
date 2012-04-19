kernel: kernel.o
	i586-elf-ld -o kernel boot.o kernel.o

kernel.o: kernel.c boot.o
	i586-elf-gcc kernel.c -c

boot.o: boot.S
	i586-elf-gcc boot.S -c

clean: 
	rm kernel.o kernel boot.o

