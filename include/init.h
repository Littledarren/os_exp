//初始化用的一些。
#ifndef INIT_H
#define INIT_H
#include <types.h>
//重命名函数指针
typedef void (*IRQ_FUNC)();

//GDT 表项
typedef struct
{
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_mid;
    uint8_t access_right;
    unsigned limit_high : 4;
    unsigned flags : 4;
    uint8_t base_high;
} __attribute__((packed)) SegmentDescriper;

//中断表项
typedef struct
{
    uint16_t offset_low, selector;
    uint8_t dw_count, access_right;
    uint16_t offset_high;
} __attribute__((packed)) GateDescriper;

//初始化
int init();
//初始化GDT表项
void init_GDT();
//初始化IDT表项
void init_IDT();
//初始化IRQ
void init_IRQ();
//设置自定义中断。
IRQ_FUNC setIRQ(int num, IRQ_FUNC new_irq);

//设置IDT。
void setGateDesc(GateDescriper *IDT, int offset, int selector, int ar);
//需要设置GDT实现重定位。
void setSegDesc(SegmentDescriper *GDT, unsigned int limit, int base, int ar);
//分配GDT
int alloc_gdt();
//释放GDT
void free_gdt(int gdt);
#endif