#include <drv/kbd.h>
#include <asm.h>
#include <interrupt.h>
#include <stdio.h>

static char KBD_ascii[] = {
    ' ', ' ', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', ' ',
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '~', ' ', '\\',
    'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', ' ', ' ', ' ',
    ' '};

//只会转换KEY_DOWN码。对KEY_UP不会理会。
static char code2ascii(char code)
{
    char result;
    if (code >= 0x02 && code < 0x39)
    {
        result = KBD_ascii[(int)code];
        if (result >= 'a' && result <= 'z')
        {
            result += 'A' - 'a';
        }
        if (result == ' ')
        {
            //说明在字符表中得到的ascii不对。
            //简化处理，直接返回 0
            result = 0;
        }
    }
    else if (code == 0x39)
    {
        result = ' ';
    }
    else
    {
        result = 0;
    }
    return result;
}

//存放扫描码的缓冲区
static char buffer[MAX_BUFFER];
//循环队列
static int s_index = 0;
static int e_index = 0;

void kbd()
{
    out_byte(PIC0_OCW2, 0x61);
    //循环队列
    int next_e_index = (e_index + 1) % MAX_BUFFER;
    if (next_e_index == s_index)
    {
        //缓冲区满，忽略输入。
        in_byte(0x60);
        return;
    }
    else
    {
        buffer[e_index] = in_byte(0x60);
    }
    e_index = next_e_index;
}

char kbd_getchar()
{
    sti();
    while (s_index == e_index)
        ;
    cli();
    int scanCode = buffer[s_index];
    s_index = (s_index + 1) % MAX_BUFFER;
    sti();
    return code2ascii(scanCode);
}
int kbd_isInput()
{
    return s_index != e_index;
}

void kbd_init()
{
    s_index = 0;
    e_index = 0;

    setIRQ(1, kbd);
    return;
}