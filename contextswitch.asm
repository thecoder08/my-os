extern oldEsp
extern newEsp
global ctxsw
extern newTaskEntry
extern newTaskStack
global newtask

section .text

ctxsw:
    pushad ; save GP-registers to thread stack
    pushfd
    mov [oldEsp], esp ; save esp
    mov esp, [newEsp] ; load new esp
    popfd
    popad ; load GP-registers from thread stack
    ret

newtask:
    mov [oldstack], esp
    mov esp, [newTaskStack]
    mov eax, [newTaskEntry]
    push eax ; EIP
    pushad
    pushfd
    mov esp, [oldstack]
    ret

section .bss
oldstack: resd 1