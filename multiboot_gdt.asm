global new_gdt
extern gdtDescriptor
extern print

MB1_MAGIC    equ 0x1badb002
MB1_FLAGS    equ 0x00000007 ; 0x00000007 for graphics
MB1_CHECKSUM equ -(MB1_MAGIC+MB1_FLAGS)

section .text
align 4
    dd MB1_MAGIC
    dd MB1_FLAGS
    dd MB1_CHECKSUM
    dd 0,0,0,0,0
    dd 0 ; linear graphics
    dd 1024 ; width
    dd 768 ; height
    dd 32 ; depth

; this sets up our segment registers with the new gdt and jumps into the new code segment
new_gdt:
lgdt [gdtDescriptor] ; load gdt

mov ax, 0x10 ; set up segment registers
mov ds, ax
mov ss, ax
mov es, ax
mov fs, ax
mov gs, ax

jmp 0x08:done ; far jump (sets cs)
done:
push gdtMsg
call print
add esp, 4
ret

section .rodata
gdtMsg: db "Loaded GDT", 13, 10, 0