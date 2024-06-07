section .text
extern print
extern scan
print:
    mov ecx, 0
    mov edx, [esp+4]
    int 0x80
    ret
scan:
    mov ecx, 1
    mov edx, [esp+4]
    int 0x80
    ret