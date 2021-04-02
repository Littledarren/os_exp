//屏幕驱动程序。。


#ifndef SCREEN_H
#define SCREEN_H

#include <stddef.h>
#include <stdarg.h>
#include <types.h>

//不建议使用的函数
//将输入位置移动到指定位置，通过行列号指定
void screen_moveXY(int lineNum, int columnNum);
//获取当前输入位置的指针
char *screen_getCurrent_cursor();
//设置输入指针
void screen_setCursor(char * cursor);

//设置颜色代码
void screen_setColor(char new_color);
//获取颜色代码
char screen_getColor();
//将buf中的内容输出到屏幕上，无格式化,返回输出的字符数
size_t screen_write(char * buf, size_t size);
//读取屏幕上的内容(字符)到buf，读size字节
size_t screen_read(char *buf, size_t size);
//写一行
size_t screen_write_line(char *buf, size_t size);
//写一个字符
void screen_write_char(char ch, char custom_color);
//换行
void screen_newline();
//向下滚屏n行
void screen_row(int n);
//清屏
void screen_cls();
//屏幕初始化
void screen_init();


//输入地址
#define VRAM_START ((char *)(0xb8000))
//行列号限制
#define LINE_MAX 25
#define COLUMN_MAX 80

#endif