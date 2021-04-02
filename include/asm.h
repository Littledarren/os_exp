//一些汇编接口
#ifndef ASM_H
#define ASM_H
#include <types.h>

uint8_t in_byte(uint16_t port);

void out_byte(uint16_t port, uint8_t data);

uint16_t in_word(uint16_t port);

void out_word(uint16_t port, uint16_t data);

void cli();

void sti();

void hlt();


#endif