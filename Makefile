C_SRC = $(wildcard *.c)
ASM_SRC = $(filter-out multiboot_gdt.asm, $(wildcard *.asm))
OBJ = $(ASM_SRC:.asm=.o) $(C_SRC:.c=.o)

run: myos.img
	qemu-system-i386 --gdb tcp::1234 -m 4G -hda $^

myos.img: kernel.elf grub.cfg myos.sfdisk pgms
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

pgms:
	$(MAKE) -C programs

kernel.elf: multiboot_gdt.o ${OBJ}
	ld $^ -melf_i386 -e kmain -Ttext 0x0 -o $@

%.o: %.asm
	nasm -g -f elf32 $^

%.o: %.c
	gcc -ffreestanding -g -m32 -Wall -c $^ -mgeneral-regs-only -fno-pie -fno-stack-protector

clean:
	rm -rf *.o *.elf *.img build
	$(MAKE) -C programs clean
