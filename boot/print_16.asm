

print_16:
    pusha 
    mov si, ax
    mov ah, 0x0e
.loop:
    mov al, [si]
    inc si 
    cmp al, 0
    je .return
    mov bx, 15
    int 0x10
    jmp .loop
.return:
    popa
    ret
