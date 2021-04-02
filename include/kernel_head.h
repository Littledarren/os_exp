
#ifndef KERNEL_HEAD_H
#define KERNEL_HEAD_H
//一次性包含所有头文件
#include <asm.h>
#include <debug.h>
#include <fs.h>
#include <init.h>
#include <interrupt.h>
#include <mm.h>
#include <sched.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <types.h>





//来自loader.asm
//中断服务程序
extern void _irq__0();
extern void _irq__1();
extern void _irq__2();
extern void _irq__3();
extern void _irq__4();
extern void _irq__5();
extern void _irq__6();
extern void _irq__7();
extern void _irq__8();
extern void _irq__9();
extern void _irq__10();
extern void _irq__11();
extern void _irq__12();
extern void _irq__13();
extern void _irq__14();
extern void _irq__15();

//int 0x30 --->
extern void _irq__16();
extern void _irq__17();
extern void _irq__18();
extern void _irq__19();



//init.c  -------end----------




#endif