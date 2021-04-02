#include <stdio.h>
#include <types.h>
#include <drv/screen.h>
#include <asm.h>

char colors[] = {
    0b01000010, //red , green
    0b11000010, //red flash, green
    0b01001010, //red , highlight green
    0b00000111, //black, white
    0b01110001, //white, blue
};

// void prog()
// {
//     cls();
//     puts("user 1,2,3,4 to interrupt,  other key to return");
//     while (1)
//     {
//         char ch = getchar();
//         switch (ch - '0')
//         {
//         case 1:
//             asm("int $0x30");
//             break;
//         case 2:
//             asm("int $0x31");
//             break;
//         case 3:
//             asm("int $0x32");
//             break;
//         case 4:
//             asm("int $0x33");
//             break;
//         default:
//             return;
//         }
//     }
// }

char turn[] = {'S', 'Y', 'S', 'U'};
void prog1()
{
    int x = 7;
    int y = 1;
    int v_x = 1;
    int v_y = 1;
    int i = 0;
    //循环
    long long timer = 500000;
    long long counter;
    while (!test('Q'))
    {

        counter = 0;
        while (counter < timer)
            counter++;
        cli();
        screen_moveXY(x, y);
        screen_write_char(turn[i++], x + y);
        sti();
        i %= 4;

        int next_x = x + v_x;
        int next_y = y + v_y;
        if (next_x >= 13 || next_x <= -1)
        {
            v_x = -1 * v_x;
            next_x += 2 * v_x;
        }

        if (next_y >= 40 || next_y <= -1)
        {
            v_y = -1 * v_y;
            next_y += 2 * v_y;
        }

        x = next_x;
        y = next_y;
    }
    cls();
}

void prog2()
{
    uint16_t *word = (uint16_t *)0xb8000;
    uint16_t i = 0;
    int x = 0, y = 41;

    while (!test('Q'))
    {
        i++;
        uint16_t *temp = word + x * 80 + y;
        cli();
        *temp++ = i;
        sti();
        y++;
        if (y > 79)
        {
            y = 41;
            x++;
            x %= 14;
        }
    }
    cls();
}
void prog3()
{
    //tantantan
    //初始化
    int x = 14;
    int y = 1;
    int v_x = 1;
    int v_y = 1;
    int i = 0;
    //循环
    long long timer = 500000;
    long long counter;
    while (!test('Q'))
    {

        counter = 0;
        while (counter < timer)
            counter++;

        cli();
        screen_moveXY(x, y);
        screen_write_char(turn[i++], x + y);
        sti();
        i %= 4;

        int next_x = x + v_x;
        int next_y = y + v_y;
        if (next_x >= 25 || next_x <= 12)
        {
            v_x = -1 * v_x;
            next_x += 2 * v_x;
        }

        if (next_y >= 40 || next_y <= -1)
        {
            v_y = -1 * v_y;
            next_y += 2 * v_y;
        }

        x = next_x;
        y = next_y;
    }
    cls();
}

void prog4()
{
    int x = 14;
    int y = 40;
    int v_x = 1;
    int v_y = 1;
    int i = 0;
    //循环
    long long timer = 500000;
    long long counter;
    while (!test('Q'))
    {

        counter = 0;
        while (counter < timer)
            counter++;

        cli();
        screen_moveXY(x, y);
        screen_write_char(turn[i++], x + y);
        sti();
        i %= 4;

        int next_x = x + v_x;
        int next_y = y + v_y;
        if (next_x >= 24 || next_x <= 12)
        {
            v_x = -1 * v_x;
            next_x += 2 * v_x;
        }

        if (next_y >= 80 || next_y <= 40)
        {
            v_y = -1 * v_y;
            next_y += 2 * v_y;
        }

        x = next_x;
        y = next_y;
    }
    cls();
}

extern int fork();
extern void exit();
extern int _wait();
extern void exit_para(char ch);

void prog5()
{

    int id = fork();
    if (id == 0)
    {
        prog1();
        exit();
    }
    id = fork();
    if (id == 0)
    {
        prog2();
        exit();
    }
    id = fork();
    if (id == 0)
    {
        prog3();
        exit();
    }

    prog4();
    //其实执行不到这里。。
    //cls();
}

char str[80] = "129djwqhdsajd128dw39ie93i8494urjoiew98kdkd";
int LetterNr = 0;
int CountLetter(char *str);
//如果在父进程等待之前就返回，会导致父进程阻塞。
void prog6()
{
    int pid;
    char ch;
    pid = fork();
    if (!pid)
    {
        //1号线程
        pid = fork();

        if (!pid)
        {
            //2号线程
            LetterNr = CountLetter(str);
            exit_para(0);
        }
        ch = _wait();
        puts("LetterNr = ");
        printHEX(LetterNr);
        exit();
    }
    //idle;
    while (!test('Q'))
        ;
}
int CountLetter(char *str)
{
    int sum = 0;
    while (*str != '\0')
    {
        if ((*str >= 'a' && *str <= 'z') || (*str >= 'A' && *str <= 'Z'))
        {
            sum++;
        }
        str++;
    }
    return sum;
}
