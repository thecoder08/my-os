global read
global write
global exit
section .text
read:
mov rax, 0
syscall
ret
write:
mov rax, 1
syscall
ret
exit:
mov rax, 60
syscall
