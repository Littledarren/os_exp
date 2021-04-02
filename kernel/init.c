#include <asm.h>
#include <stdio.h>
#include <kernel_head.h>
#include <stdarg.h>

#include <init.h>

#define EXT_MEM_KB (*(unsigned short *)0x7c02)
#define DRIVE_INFO (*(struct drive_info *)0x7c10)

struct drive_info
{
    char dummy[32];
} drive_info;

static long memory_end = 0;
static long buffer_memory_end = 0;
static long main_memory_start = 0;

int init()
{
    drive_info = DRIVE_INFO;
    memory_end = (1 << 20) + (EXT_MEM_KB << 10);
    memory_end &= 0xfffff000;
    buffer_memory_end = 4 * 1024 * 1024;
    main_memory_start = buffer_memory_end;
    // ;----------------------------------------------------------------------------------------------------
    //  内存分配：
    //  0x00000000-0x000fffff:(1M)实模式用,另外栈也设置在这里面。页表占据前五页
    //  0x00100000-0x00267fff:用于保存软盘的内容(1440KB)，当然实际没有用到这么多。
    //  0x00268000-0x0026f7ff:空(30KB)
    //  0x0026f800-0x0026ffff:IDT(2KB)
    //  0x00270000-0x0027ffff:GDT(64KB)
    //  0x00280000-0x002fffff:其他
    //  0x00300000-0x003fffff:其他(1MB)
    //  0x00400000-                     :空
    // -------

    //1：初始化GDT
    init_GDT();
    //2：初始化IDT
    init_IDT();
    //3：初始化IRQ
    init_IRQ();
    //4：初始化标准IO
    STDIO_init();
    //5：初始化调度程序
    schedINIT();
    

    return 0;
}