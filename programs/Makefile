all: shell.bin test.bin

shell.bin: shell
	objcopy -O binary -j .text -j .rodata -j .data -j .bss $^ $@
	chmod -x $@

shell: shell.o syscall.o ../string.o
	ld $^ -melf_i386 -Ttext 0x0 -Trodata 0x1000 -Tdata 0x2000 -Tbss 0x3000 -o $@

%.o: %.c
	gcc -ffreestanding -fPIC -m32 -Wall -c $^ -o $@

test.bin: test
	objcopy -O binary -j .text -j .rodata -j .data -j .bss $^ $@
	chmod -x $@

test: test.o syscall.o
	ld $^ -melf_i386 -Ttext 0x0 -Trodata 0x1000 -Tdata 0x2000 -Tbss 0x3000 -o $@

syscall.o: syscall.asm
	nasm -f elf32 $^

clean:
	rm -f *.bin shell test *.o
