SRC = $(wildcard *.c)
OBJ = ${SRC:.c=.o}

run: kernel.elf
	qemu-system-i386 -kernel $^


kernel.elf: multiboot.o ${OBJ}
	ld -melf_i386 -e kmain -Tdata=0 -Ttext=100 $^ -o $@

multiboot.o: multiboot.asm
	nasm -f elf32 $^ -o $@

%.o: %.c
	gcc -m32 -c $< -o $@

clean:
	rm -f *.o *.elf