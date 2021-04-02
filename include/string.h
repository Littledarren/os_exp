#ifndef STRING_H
#define STRING_H

#include <types.h>
//拷贝
void *memcpy(void *dest, const void *src, size_t n);
//移动,其实一样的
void *memmove(void *dest, const void *src, size_t n);
//设位
void *memset(void *dest, uint8_t num, size_t n);

//字符串长度
int strlen(const char *str);
//字符串比较
int strcmp(const char *left, const char *right);
//清空白字符
void strip(char *str);
//n位比较
int strncmp(char *left, char *right, int n);

//？？？
int isdigit(char ch);
int isalpha(char ch);
#endif