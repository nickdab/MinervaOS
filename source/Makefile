kernel: kernel.o
	i586-elf-ld -o kernel kernel.o

kernel.o: kernel.c
	i586-elf-gcc kernel.c -c

clean: 
	rm kernel.o kernel
