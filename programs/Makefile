test.img: shell.bin
	dd if=/dev/zero of=test.img bs=512 count=17
	dd if=shell.bin of=test.img conv=notrunc

shell.bin: shell
	objcopy -O binary -j .text -j .rodata -j .data -j .bss $^ $@
	chmod -x $@

shell: shell.o ../string.o ../terminal.o ../vga.o ../serial.o ../io.o
	ld $^ -melf_i386 -Ttext 0x10000 -Trodata 0x11000 -Tdata 0x12000 -Tbss 0x13000 -o $@

shell.o: shell.c
	gcc -ffreestanding -m32 -Wall -c $^ -o $@

clean:
	rm -f shell.bin shell *.o
