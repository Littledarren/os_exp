//键盘驱动。。。控制键盘中断和简单的输入

#ifndef KBD_H
#define KBD_H

#include <stddef.h>
#include <stdarg.h>
#include <types.h>

//两个宏函数，将扫描码转换成两个，一落一起
#define KEY_UP(code) \
    ((char)((code >> 8) & 0xff))

#define KEY_DOWN(code) \
    ((char)(code & 0xff))

// 这个函数没有必要公开
// //只会转换KEY_DOWN码。对KEY_UP不会理会。
// char code2ascii(char code);

//键盘中断的缓冲区大小
#define MAX_BUFFER 128
//键盘中断程序
void kbd();
//从键盘获取一个字符
char kbd_getchar();
//判断是否有输入
int kbd_isInput();
//初始化键盘中断。
void kbd_init();

//下面是键盘扫描码

#define KEY_ESC 0x0100
#define KEY_F1 0x3b00
#define KEY_F2 0x3c00
#define KEY_F3 0x3d00
#define KEY_F4 0x3e00
#define KEY_F5 0x3f00
#define KEY_F6 0x4000
#define KEY_F7 0x4100
#define KEY_F8 0x4200
#define KEY_F9 0x4300
#define KEY_F10 0x4400

#define KEY_NUM_1 0x0231
#define KEY_NUM_2 0x0332
#define KEY_NUM_3 0x0433
#define KEY_NUM_4 0x0534
#define KEY_NUM_5 0x0635
#define KEY_NUM_6 0x0736
#define KEY_NUM_7 0x0837
#define KEY_NUM_8 0x0938
#define KEY_NUM_9 0x0a39
#define KEY_NUM_0 0x0b30
#define KEY_HYPHEN 0x0c2d
#define KEY_EQUAL 0x0d3d
#define KEY_BACKSPACE 0x0e08
#define KEY_Tab 0x0f09
#define KEY_q 0x1071
#define KEY_w 0x1177
#define KEY_e 0x1265
#define KEY_r 0x1372
#define KEY_t 0x1474
#define KEY_y 0x1579
#define KEY_u 0x1675
#define KEY_i 0x1769
#define KEY_o 0x186f
#define KEY_p 0x1970
#define KEY_L_SPUAREBRACKET 0x1a5b
#define KEY_R_SPUAREBRACKET 0x1b5d
#define KEY_ENTER 0x1c0d

#define KEY_a 0x1e61
#define KEY_s 0x1f73
#define KEY_d 0x2064
#define KEY_f 0x2166
#define KEY_g 0x2267
#define KEY_h 0x2368
#define KEY_j 0x246a
#define KEY_k 0x256b
#define KEY_l 0x266c
#define KEY_SEMICON 0x273b
#define KEY_SINGLE_QUOTE 0x2827
#define KEY_WAVE 0x2960

#define KEY_BACKSLACE 0x2b5c
#define KEY_z 0x2c7a
#define KEY_x 0x2d78
#define KEY_c 0x2e63
#define KEY_v 0x2f76
#define KEY_b 0x3062
#define KEY_n 0x316e
#define KEY_m 0x326d
#define KEY_COMMA 0x332c
#define KEY_DOT 0x342e
#define KEY_SLACE 0x352f

#define KEY_SPACE 0x3920
#define KEY_L_Win 0xe05b
#define KEY_R_Win 0xe05c
#define KEY_Menu 0xe05d

#endif