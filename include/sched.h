//sched.c
#ifndef SCHED_H
#define SCHED_H
//定义了五个状态，但是实际上不需要那么多。
typedef enum{
    RUN,
    STOP,
    BLOCKED,
    READY,
    CREATE
}STATE;

typedef struct{
    int eax;
    int ebx;
    int ecx;
    int edx;
    int edi;
    int esi;
    int esp;
    int ebp;
    int cs;
    int eip;
    int ss;
    int ds;
    int es;
    int fs;
    int gs;
    int eflags;
} REGS;

typedef struct {
    unsigned valid : 1;
    int id;
    REGS regs;
    STATE state;
    //通过这个状态将子进程的状态返回给父进程。！
    char status;
    void* startAddr;
    void* endAddr;
    int fpcb;
    int gdt_code;
    int gdt_data;
    
} PCB;

//时钟中断的处理程序
void timer();
//调度程序初始化
void schedINIT();


//四个系统调用。

//创建线程
void sys_fork_stack();

//弃用
// void sys_fork();

//销毁线程
void sys_exit_stack();
//阻塞本线程
void sys_wait();
//解除父线程的阻塞。并返回一个字符。
void sys_exit(char ch);


void sleep();
void wake();


#define MAX_PROCESSES 20

#endif