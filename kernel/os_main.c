//全能头文件
#include <kernel_head.h>
#include <drv/screen.h>




extern void shell();


int os_main(void)
{
    init();
    sti();
    cls();
    
    shell();
    //子线程不能再派生子线程。因为fork是写死了的。要想也可以。
    while(1);
    return 0;
}








// ;----------------------------------------------------------------------------------------------------
//  内存分配：
//  0x00000000-0x000fffff:(1M)实模式用,另外内核临时栈也设置在这里面。页表占据前五页
//  0x00100000-0x00267fff:用于保存软盘的内容(1440KB)，当然实际没有用到这么多。栈设置在这里
//  0x00268000-0x0026f7ff:空(30KB)
//  0x0026f800-0x0026ffff:IDT(2KB)
//  0x00270000-0x0027ffff:GDT(64KB)
//  0x00280000-0x002fffff:其他
//  0x00300000-0x003fffff:其他(1MB)          
//  0x00400000-                     :空
// -------