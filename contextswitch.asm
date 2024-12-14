global ctxsw
global newtask
extern espVal
extern contextSwitch
extern newTaskEntry
extern newTaskStack

section .text

ctxsw:
    cli
    pushad ; save GP-registers to thread stack
    ; pushfd ; We don't need this; eflags is pushed automatically when interrupt occurs (just like eip)
    mov [espVal], esp ; save esp
    call contextSwitch
    mov esp, [espVal] ; load new esp
    popad ; load GP-registers from thread stack
    sti
    iret

newtask:
    mov [oldstack], esp
    mov esp, [newTaskStack]
    pushfd ; EFLAGS
    push cs ; CS
    push DWORD [newTaskEntry] ; EIP
    push DWORD 0 ; EAX
    push DWORD 0 ; ECX
    push DWORD 0 ; EDX
    push DWORD 0 ; EBX
    push DWORD 0 ; ESP (this seems wrong but popad discards esp)
    push DWORD [newTaskStack] ; EBP (I don't know if this is right)
    push DWORD 0 ; ESI
    push DWORD 0 ; EDI
    mov esp, [oldstack]
    ret

section .bss
oldstack: resd 1