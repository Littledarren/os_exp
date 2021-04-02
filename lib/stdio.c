#include <stdio.h>
#include <drv/kbd.h>
#include <drv/screen.h>
#include <string.h>

//很像键盘，但是是不一样的。
static char buffer[STDIN_BUFFER_SIZE];

//指示结束
static int end_of_input_index = 0;
//循环队列指示
static int s_index = 0;
static int e_index = 0;

static int endChar = '\n';

static FLAG flag;

void setEndCharEnable()
{
    flag.endCharEnable = 1;
}
void clearEndCharEnable()
{
    flag.endCharEnable = 0;
}

void setEcho()
{
    flag.echo = 1;
}
void clearEcho()
{
    flag.echo = 0;
}

int getEndChar()
{
    return endChar;
}

void setEndChar(int endch)
{
    endChar = endch;
}
const char *getSTDIN_BUFFER()
{
    return buffer;
}

//判断循环队列是否空或者满
static int isEmpty()
{
    return s_index == e_index;
}
static int isFull()
{
    int next_e_index = (e_index + 1) % STDIN_BUFFER_SIZE;
    return next_e_index == s_index;
}

void sync()
{
    end_of_input_index = e_index;
}
void clear()
{
    //清空队列
    s_index = e_index;
    sync();
}

//从键盘缓冲区输入到输入缓冲区
static void get_kbdInput()
{
    //防止阻塞
    while (!isFull() && kbd_isInput())
    {
        char temp = kbd_getchar();

        if (temp == 0)
        {
            //说明是不可知字符
            continue;
        }
        else if (temp == '\b' && flag.endCharEnable)
        {
            //只有在结束字符可用时，才需要对输入缓冲区操作。否则简化操作
            if (e_index == end_of_input_index || isEmpty()) //后面这个判断可能时多余的。
            {
                continue;
            }
            else
            {
                //从输入缓冲区删去一个字符
                e_index--;
                e_index = e_index < 0 ? e_index + STDIN_BUFFER_SIZE : e_index;
                if (flag.echo)
                {
                    //回显
                    putchar(temp);
                }
            }
        }
        else
        {
            if (flag.echo)
            {
                //回显
                putchar(temp);
            }
            buffer[e_index++] = temp;
            e_index %= STDIN_BUFFER_SIZE;
            //也就是当输入为回车时，允许输入
            if (!flag.endCharEnable || temp == endChar)
            {
                sync();
            }
        }
    }

    if (isFull())
    {
        sync();
    }
}

char getch()
{
    clearEcho();
    clearEndCharEnable();

    while (get_kbdInput(), end_of_input_index == s_index)
        ;
    char temp = buffer[s_index++];
    s_index %= STDIN_BUFFER_SIZE;

    setEcho();
    setEndCharEnable();
    return temp;
}

char getchar()
{
    //首先从键盘输入读到输入缓冲,同时判断是否为空
    while (get_kbdInput(), end_of_input_index == s_index)
        ;
    char temp = buffer[s_index++];
    s_index %= STDIN_BUFFER_SIZE;
    return temp;
}

void putchar(char data)
{
    screen_write_char(data, 0x07);
}
//gets会吃回车符
void gets(char *buffer, int buffer_size)
{
    int index = 0;
    char temp;
    while ((temp = getchar()) != '\n' && index < buffer_size)
    {
        buffer[index++] = temp;
    }
    buffer[index] = 0;
}

int puts(const char *str)
{
    int count = 0;
    while (*str != 0)
    {
        putchar(*str++);
        count++;
    }
    return count;
}

int test(char data)
{
    int result = 0;
    //这样做当然是很即时的。但是会破坏输入缓冲区。。。
    if (get_kbdInput(), s_index != e_index)
    {
        while (s_index != e_index)
        {
            if (buffer[s_index++] == data)
            {
                result = 1;
                s_index %= STDIN_BUFFER_SIZE;
                break;
            }
        }
        //总是清空缓冲区。
        clear();
    }
    return result;
}
void cls()
{
    screen_cls();
}
void STDIO_init()
{
    screen_init();
    kbd_init();
    setEcho();
    setEndCharEnable();
}

void printHEX(int data)
{
    puts("0x");
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
        putchar(buffer[i]);
    }
    putchar('\n');
}

//垃圾框框
typedef enum {
    UP,
    RIGHT,
    BOTTOM,
    LEFT
} STATE;
static STATE state = UP;
static char chatSet[] = {
    'S', 'Y', 'S', 'U'};
static int k = 0;
static unsigned char border_color = 0;

// static int SYS = 1;

void show_border()
{
    // if (!SYS)
    // {
    //     return;
    // }
    char *oldCursor = screen_getCurrent_cursor();
    int j = 4;
    char oldColor = screen_getColor();

    screen_setColor(border_color);
    while (j--)
    {
        switch (state)
        {
        case UP:
            for (int i = 0; i < 80; i++)
            {
                screen_moveXY(0, i);
                putchar(chatSet[k]);
            }
            state = RIGHT;
            break;
        case RIGHT:
            for (int i = 0; i < 25; i++)
            {
                screen_moveXY(i, 79);
                putchar(chatSet[k]);
            }
            state = BOTTOM;
            break;
        case BOTTOM:
            for (int i = 79; i >= 0; i--)
            {
                screen_moveXY(24, i);
                putchar(chatSet[k]);
            }
            state = LEFT;
            break;
        case LEFT:
            for (int i = 24; i >= 0; i--)
            {
                screen_moveXY(i, 0);
                putchar(chatSet[k]);
            }
            state = UP;
            break;
        default:
            //error
            break;
        }
    }
    k++;
    k %= 4;
    border_color++;
    screen_setColor(oldColor);
    screen_setCursor(oldCursor);
}
