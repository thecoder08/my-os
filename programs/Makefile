shell.bin: shell
	objcopy -O binary -j .text -j .rodata -j .data -j .bss $^ $@
	chmod -x $@

shell: shell.o syscall.o ../string.o
	ld $^ -melf_i386 -Ttext 0x10000 -Trodata 0x11000 -Tdata 0x12000 -Tbss 0x13000 -o $@

shell.o: shell.c
	gcc -ffreestanding -m32 -Wall -c $^ -o $@

syscall.o: syscall.asm
	nasm -f elf32 $^

clean:
	rm -f shell.bin shell *.o
