
;代码段选择子，第二个GDT表项
SEL_KERN_CODE   EQU 0x8
;第三个
SEL_KERN_DATA   EQU 0x10

[bits 32]

[section .text]

[extern os_main]

[global start]

start:

    xor eax, eax

    mov ax, SEL_KERN_DATA
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov fs, ax
    mov gs, ax
    
;为了后面。需要把堆栈设置在4M的地方

    mov esp, 0x00268000




;需要把引导程序中载入到内存的内核代码（包括这部分代码）载入到0x10000
;其实用前1M内存也可以。但是有一部分会映射到BIOS和VRAM。可能会不小心写到。
;这段代码实际上在linkscript中指定了载入地址为0x10000，所以在jmp到该地址之前，所有偏移地址实际上是错误的。简单说，不能直接用

[extern kernstart]
[extern kernend]

    mov eax, kernend
    mov ecx, kernstart
    sub eax, ecx
    mov ecx, eax

    mov esi, 0x8000
    mov edi, 0x100000
    cld
    rep movsb       ;把esi起始的若干字节传输到0x10000
    ;运输完后必须跳转到载入到对应地址的地方运行

    ;这个go在0x10000后
    jmp dword SEL_KERN_CODE:go

;----------------以上代码起始于0x8000-------------------
;----------------以下代码起始于0x10000------------------
go:
    ;对数字协处理器的检测
    mov eax, cr0
    and eax, 0x80000011
    or eax, 2
    mov cr0, eax
    call check_x87
    call setup_paging
    jmp main
check_x87:
    fninit 
    fstsw ax
    cmp al, 0
    je f1
    mov eax, cr0
    xor eax, 6
    mov cr0, eax
    ret

align 2
f1:    db 0xdb, 0xe4
    ret

pg_dir equ 0x0000
pg0 equ 0x1000
pg1 equ 0x2000
pg2 equ 0x3000
pg3 equ 0x4000
pg4 equ 0x5000

align 2
;二级页表
;页表目录 + 4 * 页表 -> 5 * 4K
;页表中的项都是4 Bytes
setup_paging:
    mov ecx, 5 * 1024
    xor eax, eax
    xor edi, edi           ;对前五页内存清零
    cld
    rep stosd

    mov dword [pg_dir], pg0 + 7
    mov dword [pg_dir + 4], pg1 + 7
    mov dword [pg_dir + 8], pg2 + 7
    mov dword [pg_dir + 12], pg3 + 7

    mov edi, pg3 + 4092
    mov eax, 0xfff007
    std 
b1:    
    stosd
    sub eax, 0x1000
    jge b1

    ;页表设置完毕。
    
    xor eax, eax
    mov cr3, eax
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax
    ret
main:
    call os_main          
;不应该返回。。。。这句话其实linus也说过。hhh

fin:    
    hlt
    jmp fin


%include "asm.asm"

