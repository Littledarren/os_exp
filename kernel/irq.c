
#include <kernel_head.h>
#include <asm.h>
#include <sched.h>

extern const int IDTAddress;

extern int SYS_FUNC;
extern int SYS_CODE;
//===================================================================================

//保存IRQ例程的处理函数指针
static IRQ_FUNC func[16] = {
    NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL};

static void irq_remap()
{
    out_byte(PIC0_ICW1, 0x11);
    out_byte(PIC1_ICW1, 0x11);

    out_byte(PIC0_ICW2, 0x20);
    out_byte(PIC1_ICW2, 0x28);

    out_byte(PIC0_ICW3, 4);
    out_byte(PIC1_ICW3, 2);

    out_byte(PIC0_ICW4, 0x01);
    out_byte(PIC1_ICW4, 0x01);

    //这里是中断掩码
    out_byte(PIC0_IMR, 0xfc);
    out_byte(PIC1_IMR, 0xff);
}

void init_IRQ()
{
    irq_remap();
    //装入硬件中断
    setGateDesc((GateDescriper *)IDTAddress + 0x20, (int)_irq__0, 8, 0x8e);
    setGateDesc((GateDescriper *)IDTAddress + 0x21, (int)_irq__1, 8, 0x8e);
    setGateDesc((GateDescriper *)IDTAddress + 0x22, (int)_irq__2, 8, 0x8e);
    setGateDesc((GateDescriper *)IDTAddress + 0x23, (int)_irq__3, 8, 0x8e);
    setGateDesc((GateDescriper *)IDTAddress + 0x24, (int)_irq__4, 8, 0x8e);
    setGateDesc((GateDescriper *)IDTAddress + 0x25, (int)_irq__5, 8, 0x8e);
    setGateDesc((GateDescriper *)IDTAddress + 0x26, (int)_irq__6, 8, 0x8e);
    setGateDesc((GateDescriper *)IDTAddress + 0x27, (int)_irq__7, 8, 0x8e);
    setGateDesc((GateDescriper *)IDTAddress + 0x28, (int)_irq__8, 8, 0x8e);
    setGateDesc((GateDescriper *)IDTAddress + 0x29, (int)_irq__9, 8, 0x8e);
    setGateDesc((GateDescriper *)IDTAddress + 0x2a, (int)_irq__10, 8, 0x8e);
    setGateDesc((GateDescriper *)IDTAddress + 0x2b, (int)_irq__11, 8, 0x8e);
    setGateDesc((GateDescriper *)IDTAddress + 0x2c, (int)_irq__12, 8, 0x8e);
    setGateDesc((GateDescriper *)IDTAddress + 0x2d, (int)_irq__13, 8, 0x8e);
    setGateDesc((GateDescriper *)IDTAddress + 0x2e, (int)_irq__14, 8, 0x8e);
    setGateDesc((GateDescriper *)IDTAddress + 0x2f, (int)_irq__15, 8, 0x8e);

    //下面是软中断
    setGateDesc((GateDescriper *)IDTAddress + 0x30, (int)_irq__16, 8, 0x8e);
    //只保留一个软中断，通过传入的参数进一步判断系统调用的功能。统一处理。
    // setGateDesc((GateDescriper *)IDTAddress + 0x31, (int)_irq__17, 8, 0x8e);
    // setGateDesc((GateDescriper *)IDTAddress + 0x32, (int)_irq__18, 8, 0x8e);
    // setGateDesc((GateDescriper *)IDTAddress + 0x33, (int)_irq__19, 8, 0x8e);
    // setGateDesc((GateDescriper *)IDTAddress + 0x34, (int)_irq__20, 8, 0x8e);

    // setGateDesc((GateDescriper *)IDTAddress + 0x35, (int)_irq__21, 8, 0x8e);
    // setGateDesc((GateDescriper *)IDTAddress + 0x36, (int)_irq__22, 8, 0x8e);
    // setGateDesc((GateDescriper *)IDTAddress + 0x37, (int)_irq__23, 8, 0x8e);
    // setGateDesc((GateDescriper *)IDTAddress + 0x38, (int)_irq__24, 8, 0x8e);
    // setGateDesc((GateDescriper *)IDTAddress + 0x39, (int)_irq__25, 8, 0x8e);
}

IRQ_FUNC setIRQ(int num, IRQ_FUNC new_irq)
{
    cli();
    if (num > 16 || num < 0)
    {
        return NULL;
    }
    else
    {
        IRQ_FUNC old_irq = func[num];
        func[num] = new_irq;
        return old_irq;
    }
    sti();
}

//处理来自外部的硬件中断
static void irq_handler(int IRQ_num)
{
    if (func[IRQ_num - 32] != NULL)
    {
        func[IRQ_num - 32]();
    }
    else
    {
        //没有对应的处理函数
        return;
    }
}

// ;0  --> exit without telling 
// ;3  --> fork
// ;4  --> wait
// ;5  --> exit with code

static void syscall(int IRQ_num)
{
    cli();
    //printHEX(SYS_FUNC);
    switch (SYS_FUNC)
    {
    case 0:
        sys_exit_stack();
        break;
    case 1:
        sleep();
        break;
    case 2:
        wake();
        break;
    case 3:
        sys_fork_stack();
        break;
    case 4:
        sys_wait();
        break;
    case 5:
        sys_exit(SYS_CODE);
        break;
    default:
        break;
    }
    sti();
}

//通用中断处理函数
void handler(int IRQ_num)
{
    if (IRQ_num >= 0 && IRQ_num < 32)
    {
        //保留中断
    }
    if (IRQ_num >= 32 && IRQ_num < 48)
    {
        irq_handler(IRQ_num);
    }
    else if (IRQ_num == 48)
    {
        syscall(IRQ_num);
    }
    else
    {
        //未知中断
    }
    //通知中断处理完毕
    out_byte(PIC0_OCW2, 0x20);
}