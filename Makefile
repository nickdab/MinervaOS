bin/kernel: bin/kernel.o
	i586-elf-ld -o bin/kernel bin/boot.o bin/kernel.o

bin/kernel.o: source/kernel.c bin/boot.o
	i586-elf-gcc -O0 -std=gnu99 -o bin/kernel.o source/kernel.c -c

bin/boot.o: source/boot.S
	i586-elf-gcc -O0 -std=gnu99 -o bin/boot.o source/boot.S -c

clean: 
	rm bin/kernel.o bin/kernel bin/boot.o

