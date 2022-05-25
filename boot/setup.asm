
;保存系统的基本信息
BOOTINFO equ 0x7c00
;init.c可能会用
;初始化
    mov ax,0        
    mov ss,ax
    mov ds,ax
    mov es,ax 
    mov sp,0x7c00
;这部分代码源于linux的0.11版setup.s
    mov ah, 0x03
    xor bh, bh
    int 0x10
    mov [BOOTINFO], dx  ;光标信息

    mov ah, 0x88
    int 0x15
    mov [BOOTINFO + 2], ax ;扩展内存大小KB，即1M之外的大小

    mov ah, 0x0f
    int 0x10
    mov [BOOTINFO + 4], bx      ;bh -->当前页
    mov [BOOTINFO + 6], ax      ;al->显示模式  ah->字符列数

    mov ah, 0x12
    mov bl, 0x10
    int 0x10
    mov [BOOTINFO + 8], bx  ;安装的显示内存，高位保存显示状态
    mov [BOOTINFO + 10], cx ;显示卡特性参数。。

    mov si, [4 * 0x41] ;取中断向量0x41的值。hd0的值
    
    mov ax, BOOTINFO
    mov es, ax
    mov cx, 0x10
    mov di, 0x0010     ;保存在7c10---7c20
    cld
    rep movsb

    xor ax, ax
    mov es, ax

;=================================================================
;                             进入保护模式
;       1：创建并载入GDT
;       2：屏蔽中断
;       3: 打开A20
;       4: 设置CR0
;       5: 清空流水。。通常是通过jmp实现
;=================================================================

    xor eax,eax
    mov ax,ds
    shl eax,4          ;
    add eax,GDT        ; 获得GDT的物理地址

    mov dword [GdtPtr+2], eax    ; [GdtPtr + 2] <- gdt base 
    ;载入GDT
    lgdt [GdtPtr]
    mov ax,msg_gdt
    call print_16

    ;屏蔽中断,此时是可以调用软中断的。
    cli
    mov ax, msg_cli
    call print_16

    ;Fast A20
    in al,0x92   
    or al,00000010b
    out  0x92, al
    mov ax,msg_a20
    call print_16


    ;设置cr0
    mov eax,cr0
    or eax, 1
    mov cr0,eax


    ;清空流水，直接跳到0x8000
    jmp dword Selec_Code32_R0:0x8000



%include "print_16.asm"

;数据区


msg_gdt:
    db "Temp GDT loaded...",13,10,0

msg_cli:
    db "interrupt down...", 13, 10, 0

msg_a20:
    db "A20 line on...",13,10,0

; base limit ar
%macro Descriptor 3

	DW	%2 & 0xffff			    ; 段界限1      16

	DW	%1 & 0xffff				; 段基址1      16

	DB	(%1 >> 16) & 0xff		; 段基址2      8

	DW	((%2 >> 8) & 0xf00) | (%3 & 0xf0ff)	; 属性1 + 段界限2 + 属性2

	DB	(%1 >> 24) & 0xff			; 段基址3  8

%endmacro 

align 8

GDT:
;64位。。暂时用用
DESC_NULL:        Descriptor 0x00000000, 0x00000, 0x0000

DESC_CODE32_R0:   Descriptor 0x00000000, 0xfffff, 0b1100000010011010

DESC_DATA_R0:     Descriptor 0x00000000, 0xfffff, 0b1100000010010010



GdtLen  equ $ - GDT     ; GDT len

GdtPtr  dw  GdtLen - 1  ; GDT limit
        dd  0           ; GDT Base



;选择子

Selec_Code32_R0 equ     0x8
Selec_Data_R0   equ     0x10



;1024字节够了吧,这里的1024其实是指两个文件的。
times 1024 - ($ - $$) db 0
