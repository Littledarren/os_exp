#include <sched.h>
#include <interrupt.h>
#include <stddef.h>
#include <types.h>
#include <asm.h>
#include <mm.h>
#include <string.h>
#include <init.h>
#include <stdio.h>

//定义于gdt.c
extern const int GDTAddress;
//定义于asm.asm
extern int TEMP_EAX;
extern int TEMP_EBX;
extern int TEMP_ECX;
extern int TEMP_EDX;
extern int TEMP_ESI;
extern int TEMP_EBP;
extern int TEMP_EDI;
extern int TEMP_ESP;

extern int TEMP_SS;
extern int TEMP_DS;
extern int TEMP_ES;
extern int TEMP_FS;
extern int TEMP_GS;

extern int TEMP_CS;
extern int TEMP_EIP;
extern int TEMP_EFLAGS;

//所有的Pcb
static PCB pcbs[MAX_PROCESSES];

//就绪队列
static int pcb_ready[MAX_PROCESSES + 1] = {0};
static int s_index = 0;
static int e_index = 0;
//阻塞队列或者说阻塞池。
static int pcb_blocked[MAX_PROCESSES + 1] = {0};
static int pcb_blocked_s_index = 0;
static int pcb_blocked_e_index = 0;

//运行的只有一个啊。
static int run = 0;

//从就绪队列中取出下一个运行的Pid
static int getNext()
{
    int test = pcb_ready[s_index];
    if (pcbs[test].valid == 0 || s_index == e_index)
    {
        //error
        //没有可用的进程可供调度。不应该发生。
        puts("error");
        return 0;
    }
    s_index = (s_index + 1) % (MAX_PROCESSES + 1);
    return test;
}
//根据进程的状态决定入队。
static void push(int pid)
{
    switch (pcbs[pid].state)
    {
    case RUN:
    case READY:
        pcbs[pid].state = READY;
        pcb_ready[e_index] = pid;
        e_index = (e_index + 1) % (MAX_PROCESSES + 1);
        break;

    case BLOCKED:
        pcb_blocked[e_index] = pid;
        pcb_blocked_e_index = (pcb_blocked_e_index + 1) % (MAX_PROCESSES + 1);
        break;
    case STOP:
    case CREATE:
    default:
        break;
    }
}

//从阻塞队列中弹出要取消阻塞的线程。
static void getReady(int pid)
{
    while (pcb_blocked[pcb_blocked_s_index] != pid &&               //判断相等
           pcbs[pcb_blocked[pcb_blocked_s_index]].state != READY && //  判断就绪
           pcbs[pcb_blocked[pcb_blocked_s_index]].valid == 1 &&     //判断有效
           pcb_blocked_s_index != pcb_blocked_e_index)              //判断空队列
    {
        //这里可能会产生循环的问题。但是要拿出来的项不一定在队首。
        push(pcb_blocked[pcb_blocked_s_index]);
        pcb_blocked_s_index = (pcb_blocked_s_index + 1) % (MAX_PROCESSES + 1);
    }
    pcb_blocked_s_index = (pcb_blocked_s_index + 1) % (MAX_PROCESSES + 1);
}

void schedINIT()
{
    for (int i = 0; i < MAX_PROCESSES; i++)
    {
        pcbs[i].valid = 0;
        pcbs[i].fpcb = -1;
        pcbs[i].id = i;
        pcbs[i].state = READY;
        pcbs[i].startAddr = (void *)(0x0);
        pcbs[i].endAddr = (void *)(0x00268000);
        //系统的起始地址和结束地址。用于fork
        //gdt
        pcbs[i].gdt_code = 1;
        pcbs[i].gdt_data = 2;
    }
    pcbs[0].valid = 1;
    //设置时钟
    setIRQ(0, timer);
}

//将全局区的临时上下文保存到当前进程的PCB中
void save()
{

    pcbs[run].valid = 1;
    //保存中断现场到对应的PCB块中

    //八个通用寄存器
    pcbs[run].regs.eax = TEMP_EAX;
    pcbs[run].regs.ebx = TEMP_EBX;
    pcbs[run].regs.ecx = TEMP_ECX;
    pcbs[run].regs.edx = TEMP_EDX;
    pcbs[run].regs.esi = TEMP_ESI;
    //由于cs, eip, cflags,都压栈了。。。这样，可以保证进程的状态不被破坏。
    pcbs[run].regs.esp = TEMP_ESP; //+ 3 * 4;
    pcbs[run].regs.ebp = TEMP_EBP;
    pcbs[run].regs.edi = TEMP_EDI;

    //指令地址寄存器
    pcbs[run].regs.cs = TEMP_CS;
    pcbs[run].regs.eip = TEMP_EIP;
    //标志寄存器
    pcbs[run].regs.eflags = TEMP_EFLAGS;

    pcbs[run].regs.ss = TEMP_SS;
    pcbs[run].regs.ds = TEMP_DS;
    pcbs[run].regs.es = TEMP_ES;
    pcbs[run].regs.fs = TEMP_FS;
    pcbs[run].regs.gs = TEMP_GS;
    //五个数据段寄存器
}

//将run的上下文复制到全局临时上下文中。
void restart()
{
    TEMP_EAX = pcbs[run].regs.eax;
    TEMP_EBX = pcbs[run].regs.ebx;
    TEMP_ECX = pcbs[run].regs.ecx;
    TEMP_EDX = pcbs[run].regs.edx;

    TEMP_ESI = pcbs[run].regs.esi;
    TEMP_ESP = pcbs[run].regs.esp;
    TEMP_EDI = pcbs[run].regs.edi;
    TEMP_EBP = pcbs[run].regs.ebp;

    TEMP_CS = pcbs[run].regs.cs;
    TEMP_EIP = pcbs[run].regs.eip;
    //指令地址寄存器

    TEMP_EFLAGS = pcbs[run].regs.eflags;

    TEMP_SS = pcbs[run].regs.ss;
    TEMP_DS = pcbs[run].regs.ds;
    TEMP_ES = pcbs[run].regs.es;
    TEMP_FS = pcbs[run].regs.fs;
    TEMP_GS = pcbs[run].regs.gs;
}
//这里应该设置为系统调用。
void schedule()
{
    save();
    //////////////////////////////保存上下文完毕

    //调度选择合适的进程
    //总是会把当前进程放入到调度队列。
    push(run);
    int next = getNext();
    if (run == next)
    {
        //简单的优化。如果调度的进程就是当前进程，那么直接执行。
        return;
    }
    run = next;
    pcbs[run].state = RUN;
    /////////////////////////////重启动
    restart();
}

void timer()
{
    schedule();
}

//static
int alloc_pid()
{
    for (int i = 1; i < MAX_PROCESSES; i++)
    {
        if (pcbs[i].valid == 0)
        {
            pcbs[i].valid = 1;
            return i;
        }
    }
    return -1;
}

//这个是核武器。。
//会把内核拷贝一份。不合理
void sys_fork()
{
    //更新下
    save();
    printHEX(pcbs[run].regs.eip);

    int childPid = alloc_pid();

    if (childPid == -1)
    {
        //不能再多了
        return;
    }
    pcbs[childPid].state = READY;
    push(childPid);
    //设置父进程
    pcbs[childPid].fpcb = run;
    printHEX(childPid);
    //前含后不包
    //分配内存空间
    int nPages = PAGE(pcbs[run].endAddr) - PAGE(pcbs[run].startAddr);
    pcbs[childPid].startAddr = malloc_page(nPages);
    pcbs[childPid].endAddr = (void *)((int)pcbs[childPid].startAddr + nPages * 4 * 1024);
    //复制内存页

    memmove(pcbs[childPid].startAddr, pcbs[run].startAddr, nPages * 4 * 1024);
    //这里需要用到重定位，一种是通过。修改页表。
    //。。我修改GDT。不太会LDT
    pcbs[childPid].gdt_code = alloc_gdt();
    pcbs[childPid].gdt_data = alloc_gdt();
    //这里可以设置相应的保护。。。
    setSegDesc((SegmentDescriper *)GDTAddress + pcbs[childPid].gdt_code, nPages, pcbs[childPid].startAddr, 0b110010011010); //CODE
    setSegDesc((SegmentDescriper *)GDTAddress + pcbs[childPid].gdt_data, nPages, pcbs[childPid].startAddr, 0b110010010010); //DATA
    pcbs[childPid].regs = pcbs[run].regs;

    pcbs[childPid].regs.eax = 0;
    pcbs[run].regs.eax = childPid;

    //设置段选择子
    pcbs[childPid].regs.cs = pcbs[childPid].gdt_code << 3;
    pcbs[childPid].regs.ss = pcbs[childPid].gdt_data << 3;
    pcbs[childPid].regs.ds = pcbs[childPid].gdt_data << 3;
    pcbs[childPid].regs.es = pcbs[childPid].gdt_data << 3;
    pcbs[childPid].regs.fs = pcbs[childPid].gdt_data << 3;
    pcbs[childPid].regs.gs = pcbs[childPid].gdt_data << 3;
    restart();
}

//轻量级Fork，仅仅产生一个新的栈
void sys_fork_stack()
{
    //更新下
    save();
    
    int childPid = alloc_pid();
    if (childPid == -1)
    {
        //不能再多了
        restart();
        return;
    }
    pcbs[childPid].state = READY;
    push(childPid);
    //设置父进程
    pcbs[childPid].fpcb = run;
    //前含后不包
    //分配内存空间,,只分配一页，写死了这里。理论上是允许动态分配。。。不过一般够用了
    int nPages = 1;
    pcbs[childPid].startAddr = malloc_page(nPages);
    pcbs[childPid].endAddr = (void *)((int)pcbs[childPid].startAddr + nPages * 4 * 1024);
    //复制内存页
    memcpy(pcbs[childPid].startAddr, pcbs[run].endAddr - nPages * 4 * 1024, nPages * 4 * 1024);
    //debug
    // printHEX(pcbs[childPid].endAddr);
    // while(1);
    //debug
    pcbs[childPid].regs = pcbs[run].regs;
    pcbs[childPid].regs.esp = pcbs[run].regs.esp + (int)pcbs[childPid].endAddr - (int)pcbs[run].endAddr;

    pcbs[childPid].regs.eax = 0;
    pcbs[run].regs.eax = childPid;
    restart();
}
void sys_exit_stack()
{
    //释放资源
    if (run == 0)
    {
        //0号线程不允许退出。
        return;
    }
    free_page(pcbs[run].startAddr, pcbs[run].endAddr);
    pcbs[run].valid = 0;
    schedule();
}

//阻塞本线程
void sys_wait()
{
    pcbs[run].state = BLOCKED;
    push(run);
    schedule();
}
//解除父线程的阻塞。并返回一个字符。
void sys_exit(char ch)
{
    //应该是在wait处返回的。
    //将参数保存在寄存器中，而不是栈。
    //这里使用直接设置，而不是保存在status中，Linux似乎使用了那种做法，因为它的wait更加复杂一点。
    pcbs[pcbs[run].fpcb].regs.eax = ch;
    pcbs[pcbs[run].fpcb].state = READY;
    //从阻塞队列中取出
    getReady(pcbs[run].fpcb);
    //放入到就绪队列。
    push(pcbs[run].fpcb);
    //最后会释放自己的栈。blabla
    sys_exit_stack();
}

//这个不仅仅是栈的释放，还有堆的释放。
static void exit()
{
    //释放资源
    if (run == 0)
    {
        //0号线程不允许退出。
        return;
    }

    free_page(pcbs[run].startAddr, pcbs[run].endAddr);
    free_gdt(pcbs[run].gdt_code);
    free_gdt(pcbs[run].gdt_data);
    //释放资源。。。
    //这个时候怎么返回呢。。。
    // 强行调度到0进程
    run = 0;
    restart();
}

void sleep()
{
    //TODO
}

void wake()
{
    //TODO
}
