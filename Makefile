C_SRC = $(wildcard *.c)
ASM_SRC = $(wildcard *.asm)
OBJ = $(C_SRC:.c=.o) $(ASM_SRC:.asm=.o)

run: kernel.elf
	qemu-system-i386 -kernel $^

myos.img: kernel.elf grub.cfg myos.sfdisk
	dd if=/dev/zero of=$@ bs=1024 count=10240
	cat myos.sfdisk | sfdisk $@
	sudo losetup -P loop0 $@
	sudo mkfs.vfat /dev/loop0p1
	mkdir build
	sudo mount /dev/loop0p1 build
	sudo cp kernel.elf build
	sudo grub-install --boot-directory=build --target=i386-pc /dev/loop0
	sudo cp grub.cfg build/grub
	sudo umount build
	sudo losetup -d /dev/loop0
	rmdir build

kernel.elf: ${OBJ}
	ld -melf_i386 -e kmain -Tdata 0x0 -Ttext 0x1000 $^ -o $@

%.o: %.asm
	nasm -f elf32 $^ -o $@

%.o: %.c
	gcc -ffreestanding -m32 -Wall -Wno-trigraphs -c $^ -o $@

clean:
	rm -f *.o *.elf *.img build