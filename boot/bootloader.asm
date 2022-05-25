

;常量
LOADER equ 0x7e0
;16位的程序
[BITS 16]
;表示从0x7c00开始，或者说一下程序会加载到0x7c00处
org 0x7c00        
;=============================================================
;FAT12软盘专用  
    db 0xeb, 0x4e, 0x90  ;0xeb4e其实就是jmp start。90是nop
    db "Silver47"        ; 启动区名称，要求刚好8个字节
    dw 512               ; 每个扇区的大小，必须为512字节
    db 1                 ; 簇的大小，必须为1
    dw 1                 ;起始位置，一般为1
    db 2                 ; FAT的个数，必须为2
    dw 224               ; 根目录的大小，一般为224项
    dw 2880              ; 磁盘大小，2880扇区
    db 0xf0              ; 磁盘种类
    dw 9                 ; FAT的长度，9扇区
    dw 18                ; 1个磁道有几个扇区，必须是18 
    dw 2                 ;磁头数，必须
    dd 0                 ;不使用分区
    dd 2880              ;重写一次磁盘大小
    db 0, 0, 0x29        ; BootSig   
    dd 0xffffffff        ;可能是卷标号码
    db "Silver47XXX"        ;磁盘名称（11字节）
    db "FAT12   "        ; 文件系统
    times 18 db 0
;============================================================

start:
    ;初始化，使用的栈底为7c00
    mov ax,0        
    mov ss,ax
    mov ds,ax
    mov es,ax 
    mov sp,0x7c00
   
    ;vga 256色字符模式（清屏）
    mov ah, 0
    mov al, 0x03
    int 0x10

;----------------------------载入内存------------------------
; 读写软盘到(0x7e0:0)

loadloader:      
    mov bx,0    
    mov ax,LOADER  
    mov es,ax   ; es:bx 写入地址（0x7e00)
    mov ch,0    ; 柱面号
    mov dh,0    ; 磁头号
    mov cl,2    ; 扇区号
    mov dl,0x00 ; 驱动器号
readloop:
    mov si,0    ; 计错
retry:
    mov ah,0x02 ; 读入磁盘
    mov al, 1   ;一个扇区
    int 0x13

    jnc next    ;没出错跳转到next

    inc si
    cmp si,5    

    jae error   ;不可接受的错误

    mov ah,0x00 ;重置驱动器
    mov dl,0x00 
    int 0x13    

    jmp retry 


next: 

    mov ax, es
    add ax, 0x20    ; 内存地址后移0x200(512Bytes)
    mov es, ax

    inc cl          ; 扇区号加一
    cmp cl, 18      ; 18扇区
    jbe readloop


    mov cl, 1

    inc dh          ; 2个磁头
    cmp dh, 1
    jbe readloop



    mov dh, 0

    add ch, 1   ;30
    cmp ch, 30
    jb readloop

    jmp succ

error:        
    mov  ax, msg_err
    call print_16 
reboot:         ;重启
    mov ah, 0
    int 0x16
    int 0x19
    ;手动重启
    jmp 0xfff0:0000

;-------------------------------载入完毕--------------------------
succ:    
    mov ax, msg_succ
    call print_16
    jmp LOADER:0000     ;直接跳转到setup.asm





msg_err:
    db "Loading Failed\n"
    db "Press Any Key To Reboot\n"
    db 0
msg_succ:
    db "Loading Success"
    db 10, 13
    db 0

times 510 - ($-$$) db 0
db 0x55, 0xaa


;       
;       
;       
;       
;       0xa0000 --> 不能碰的地方,系统（保护模式）堆栈设置在这里。
;       
;       
;       
;       0x8100  
;       
;       0x7e00  --> start of the other files
;       
;       0x7c00  --> start of this file  把系统数据保存在这块,emm.如果有的话
;       底下的部分在实模式还有限制，不过保护模式后就只会当作栈。
;       
;       
%include "setup.asm"
