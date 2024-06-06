; this sets up our segment registers with the new gdt and jumps into the new code segment
global new_gdt
extern gdtDescriptor
extern print

section .text
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