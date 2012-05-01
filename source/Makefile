kernel: kernel.o
	i586-elf-ld -o ../bin/kernel ../bin/boot.o ../bin/kernel.o

kernel.o: kernel.c boot.o
	i586-elf-gcc -std=gnu99 -o ../bin/kernel.o kernel.c -c

boot.o: boot.S
	i586-elf-gcc -std=gnu99 -o ../bin/boot.o boot.S -c

clean: 
	rm ../bin/kernel.o ../bin/kernel ../bin/boot.o

