extern print
extern scan
extern runFile
extern listRoot
section .text
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
runFile:
    mov ecx, 2
    mov edx, [esp+4]
    int 0x80
    mov eax, esi
    ret
listRoot:
    mov ecx, 3
    int 0x80
    ret