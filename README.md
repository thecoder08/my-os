# My OS
This is a simple 32-bit OS that can run some basic commands. It is written in C and uses multiboot.

It supports:
- [x] Text mode VGA driver
- [x] PS/2 Keyboard and mouse drivers
- [x] Serial port driver
- [x] Terminal driver, with multiplexing between Serial and VGA/Keyboard
- [x] A simple shell, with support for basic commands
- [x] Basic libc with utility functions (strlen, strcmp, strcpy, strcat, reverse, itoa, memcpy, memset)
- [x] Parallel port driver
- [ ] Dynamic memory allocation (half done, currently malloc just returns an increasing pointer and cannot be free'd)
- [ ] Monitor serial port from VGA
- [ ] Floppy disk driver (In progress)
- [ ] Filesystem (Maybe FAT) (will be worked on when FDC driver is done)
- [ ] Modesetting VGA driver, with support for text and 256 color graphics modes
- [ ] Graphical Shell with terminal emulator, calculator, text editor, and a toolkit to make more applications.
- [ ] Hard disk support

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

## Getting an image
Use
```
make myos.img
```
To build a 10MB hard disk image that can be flashed to a USB drive or run in an emulator for testing. (Needs `grub2` and `grub-pc-bin`)

or

Just download an image from the Actions tab. These images are built from the latest version of the code and may or may not work.