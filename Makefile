C_SRC = $(wildcard *.c)
ASM_SRC = $(wildcard *.asm)
OBJ = $(C_SRC:.c=.o) $(ASM_SRC:.asm=.o)

run: kernel.elf
	qemu-system-i386 -kernel $^


kernel.elf: ${OBJ}
	ld -melf_i386 -e kmain -Tdata 0x0 -Ttext 0x1000 $^ -o $@

%.o: %.asm
	nasm -f elf32 $^ -o $@

%.o: %.c
	gcc -ffreestanding -m32 -c $^ -o $@

clean:
	rm -f *.o *.elf