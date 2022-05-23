# My OS
This is a simple 32-bit OS that can run some basic commands. It is written in C and uses multiboot.
## Running the OS
Use
```
make
```
or
```
make run
```
to run the OS in QEMU.

Use
```
make kernel.elf
```
To build the kernel as `kernel.elf`, a 32-bit ELF multiboot executable.

Use
```
make myos.img
```
To build a 10MB hard disk image that can be flashed to a USB drive for testing. (Needs `grub2` and `grub-pc-bin`)