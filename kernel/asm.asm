[global TEMP_EAX]
[global TEMP_EBX]
[global TEMP_ECX]
[global TEMP_EDX]
[global TEMP_ESI]
[global TEMP_EBP]
[global TEMP_EDI]
[global TEMP_ESP]

[global TEMP_SS]
[global TEMP_DS]
[global TEMP_ES]
[global TEMP_FS]
[global TEMP_GS]

[global TEMP_CS]
[global TEMP_EIP]
[global TEMP_EFLAGS]

TEMP_EAX:    dd 0
TEMP_EBX:    dd 0
TEMP_ECX:    dd 0
TEMP_EDX:    dd 0
TEMP_ESI:    dd 0
TEMP_EBP:    dd 0
TEMP_EDI:    dd 0
TEMP_ESP:    dd 0

TEMP_SS:     dd 0
TEMP_DS:     dd 0
TEMP_ES:     dd 0
TEMP_FS:     dd 0
TEMP_GS:     dd 0

TEMP_CS:     dd 0
TEMP_EIP:    dd 0
TEMP_EFLAGS: dd 0

;用于系统调用

[global SYS_FUNC]
[global SYS_CODE]
SYS_FUNC: dd 0
SYS_CODE: dd 0




[extern handler]

;保存中断号
%macro m_irq 1

[global _irq__%1]

_irq__%1:
    ;push eflags
    ;push cs
    ;push eip   <---esp
    push esp 
    push eax 
    push ebx 
    push ecx 
    push edx 
    push ebp 
    push edi 
    push esi 

    push ss
    push es
    push ds
    push fs 
    push gs

    ;进入内核全局区。
    mov ax, 0x10
    mov ds, ax
    ;保存有关信息
    pop dword [TEMP_GS]
    pop dword [TEMP_FS]
    pop dword [TEMP_DS]
    pop dword [TEMP_ES]
    pop dword [TEMP_SS]
    pop dword [TEMP_ESI]
    pop dword [TEMP_EDI]
    pop dword [TEMP_EBP]
    pop dword [TEMP_EDX]
    pop dword [TEMP_ECX]
    pop dword [TEMP_EBX]
    pop dword [TEMP_EAX]
    pop dword [TEMP_ESP]

    pop dword [TEMP_EIP]
    pop dword [TEMP_CS]
    pop dword [TEMP_EFLAGS]
   
    ;not smart
    mov eax, 16
    cmp eax, %1
    jne NOT_SYS_CALL%1
    pop dword [SYS_FUNC]
    mov eax, [SYS_FUNC]
    cmp eax, 5
    jne NOT_EXIT_PARA%1
    pop dword [SYS_CODE]
NOT_EXIT_PARA%1:
NOT_SYS_CALL%1:
    ;进入到内核栈
    ;最后保存栈，确保传入的参数都被弹出。
    mov [TEMP_ESP], esp

    mov ax, 0x10
    mov ss, ax
    mov sp, 0x7e00

    push dword %1+32   
    call handler
    pop eax

    ;到用户栈
    push dword [TEMP_SS]
    pop ss
    mov esp, [TEMP_ESP]
    
    push dword [TEMP_ES]
    pop es 
    push dword [TEMP_FS]
    pop fs
    push dword [TEMP_GS]
    pop gs
    ;这里绝对不能改ds
    mov eax, [TEMP_EAX]
    mov ebx, [TEMP_EBX]
    mov ecx, [TEMP_ECX]
    mov edx, [TEMP_EDX]
    mov esi, [TEMP_ESI]
    mov edi, [TEMP_EDI]
    mov ebp, [TEMP_EBP]
    push dword [TEMP_EFLAGS]
    push dword [TEMP_CS]
    push dword [TEMP_EIP]
    push dword [TEMP_DS]
    pop ds
    iret

%endmacro
;通过系统全局变量来传递参数。同时通过修改这些全局变量可以实现上下文的切换



;0x20 ~0x2f
%assign i 0
%rep 16 

    m_irq i   
    %assign i i+1

%endrep


;0x30 ~ 0x30 + 10
%assign i 16
%rep 10

    m_irq i
    %assign i i+1

%endrep

;0  --> exit without telling 
;3  --> fork
;4  --> wait
;5  --> exit with code


;int fork()
[global fork]

fork:
    push dword 3
    int 0x30
    ret

;void exit()
[global exit]
exit:
    push dword 0
    int 0x30
    jmp $
    ret

;int Wait()
[global _wait]
_wait:
    push dword 4
    int 0x30

    ret

;void exit(char ch)
[global exit_para]
exit_para:
    push ebp  
    mov ebp, esp
    xor eax, eax
    mov eax, [ebp + 8]
    push eax 
    push dword 5
    int 0x30
    leave
    ret









;           无特权级切换
;           EFLAGS
;           CS
;           EIP
;           ErrorCode       -->不存在


;           有特权切换
;           SS
;           ESP
;           EFLAGS
;           CS
;           EIP
;           ErrorCode





;pusha-->
; Push(EAX);
; Push(ECX);
; Push(EDX);
; Push(EBX);
; Push(ESP);这个SP是没有压栈之前的。
; Push(EBP);
; Push(ESI);
; Push(EDI);