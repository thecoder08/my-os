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
    dd 0 ; width
    dd 0 ; height
    dd 32 ; depth