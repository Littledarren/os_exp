#include <debug.h>

//保存当前光标位置
int cursor = 0;
//保存界面大小
const int MAX_X = 25;
const int MAX_Y = 80;
//VRAM
const int Start = 0xb8000;

//最大输出位数
#define MAX_LEN 20
//缓存
char buffer[MAX_LEN];

static void DEBUG_putchar(char ch)
{
    cursor += 2;
    cursor %= MAX_X * MAX_Y * 2;

    char *write = (char *)(Start + cursor);

    *write = ch;
    *(write + 1) = 13;
}

void DEBUG_print_hex(int data)
{
    DEBUG_putchar('0');
    DEBUG_putchar('x');

    int i = 0;

    while (data)
    {
        char temp = data % 16;
        if (temp >= 10)
        {
            temp -= 10;
            temp += 'A';
        }
        else
        {
            temp += '0';
        }
        buffer[i++] = temp;
        data /= 16;
    }

    while (i--)
    {
        DEBUG_putchar(buffer[i]);
    }
    DEBUG_putchar('\n');
}

void DEBUG_print_string(char *str)
{
    while (*str != 0)
    {
        DEBUG_putchar(*str++);
    }
}