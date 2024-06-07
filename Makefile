C_SRC = $(wildcard *.c)
ASM_SRC = $(wildcard *.asm)
OBJ = $(ASM_SRC:.asm=.o) $(C_SRC:.c=.o)

run: kernel.elf
	qemu-system-i386 --gdb tcp::1234 -m 4G -kernel $^

myos.img: kernel.elf grub.cfg myos.sfdisk programs/shell.bin programs/test.bin
	dd if=/dev/zero of=$@ bs=1M count=10
	cat myos.sfdisk | sfdisk $@
	sudo losetup -P loop8 $@
	sudo mkfs.fat -F16 -n MYOS /dev/loop8p1
	mkdir build
	sudo mount /dev/loop8p1 build
	sudo cp kernel.elf programs/shell.bin programs/test.bin build
	sudo grub-install --boot-directory=build --target=i386-pc /dev/loop8
	sudo cp grub.cfg build/grub
	sudo umount build
	sudo losetup -d /dev/loop8
	rmdir build

kernel.elf: ${OBJ}
	ld $^ -melf_i386 -e kmain -Ttext 0x0 -o $@

%.o: %.asm
	nasm -g -f elf32 $^

%.o: %.c
	gcc -ffreestanding -g -m32 -Wall -c $^ -mgeneral-regs-only -fno-pie

clean:
	rm -rf *.o *.elf *.img build
