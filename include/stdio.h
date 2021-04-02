#ifndef STDIO_H
#define STDIO_H

#define STDIN_BUFFER_SIZE 256

//输入结束标志符
//简单说就是，一旦回车，不许删除
// extern int endChar;
//flag.echo
typedef struct 
{
    unsigned echo : 1;
    unsigned endCharEnable: 1;
    //保留升级
} FLAG;

//可能不需要结束字符
void setEndCharEnable();
void clearEndCharEnable();

//设置清除echo位
void setEcho();
void clearEcho();
//获取终止符
int getEndChar();
//设置终止符
void setEndChar(int endch);
//获取输入缓冲区字符。
const char * getSTDIN_BUFFER();
//将循环队列中的值作为输出。
void sync();
//清空缓冲
void clear();
//不会回显，啥也不显示。不适用结束字符
char getch();
//从输入缓冲区获得一个字符!!!注意，getchar是以回车结束的！会回显
char getchar();
//向屏幕输出
void putchar(char data);
//获得字符串。
void gets(char *buffer, int buffer_size);

int puts(const char *str);
//检验是否输入了data
int test(char data);
//清屏
void cls();
//初始化标准IO
void STDIO_init();

//可能没用的函数

//显示框框。
void show_border();
//输出16进制数，比debug友好。
void printHEX(int data);


#endif