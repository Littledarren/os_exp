#include <drv/screen.h>
#include <string.h>
#include <asm.h>

//这些驱动只能在内核中使用。。。 
static char *current_cursor = VRAM_START;
static char color = 0x07;

#define LINE \
    ((current_cursor - VRAM_START) / 160)

#define COLUMN \
    (((current_cursor - VRAM_START) % 160) / 2)

void screen_moveXY(int lineNum, int columnNum)
{
    lineNum %= LINE_MAX;
    columnNum %= COLUMN_MAX;
    current_cursor = VRAM_START + (lineNum * 160 + columnNum * 2);
}

char *screen_getCurrent_cursor()
{
    return current_cursor;
}

void screen_setCursor(char *cursor)
{
    current_cursor = cursor;
}

void screen_setColor(char new_color)
{
    color = new_color;
}

char screen_getColor()
{
    return color;
}

size_t screen_write(char *buf, size_t size)
{
    size_t i;
    for (i = 0; i < size; i++)
    {
        screen_write_char(*(buf + i), color);
    }
    return i;
}

size_t screen_read(char *buf, size_t size)
{
    size_t i;
    size = size < LINE_MAX * COLUMN_MAX ? size : LINE_MAX * COLUMN_MAX;
    for (i = 0; i < size; i++)
    {
        *(buf + i) = *(VRAM_START + i);
    }
    return size;
}

size_t screen_write_line(char *buf, size_t size)
{
    screen_moveXY(LINE, 0);
    return screen_write(buf, size < COLUMN_MAX ? size : COLUMN_MAX);
}
void screen_write_char(char ch, char custom_color)
{
    if(ch == 0)
    {
        return;
    }

    if (ch == '\n')
    {
        screen_newline();
        return;
    }
    //这里没有办法！！！小心谨慎
    if(ch == '\b')
    {
        while(current_cursor != VRAM_START && *(current_cursor + 1) == 0)
        {
            current_cursor -= 2;
        }
        *(current_cursor + 1) = 0;
        return;
    }

    if (current_cursor >= LINE_MAX * COLUMN_MAX * 2 + VRAM_START)
    {
        screen_row(1);
    }
    *current_cursor = ch;
    *(current_cursor + 1) = custom_color;
    current_cursor += 2;
}
void screen_newline()
{
    if (LINE + 1 == LINE_MAX)
    {
        screen_row(1);
    }
    screen_moveXY(LINE + 1, 0);
}
void screen_row(int n)
{
    n = n < LINE_MAX ? n : LINE_MAX;
    memcpy(VRAM_START, VRAM_START + n * COLUMN_MAX * 2, (LINE_MAX - n) * COLUMN_MAX * 2);
    memset(VRAM_START + (LINE_MAX - n) * COLUMN_MAX * 2, 0, n * COLUMN_MAX * 2);
    current_cursor -= n * COLUMN_MAX * 2;
    current_cursor = current_cursor < VRAM_START ? VRAM_START : current_cursor;
}
void screen_cls()
{
    screen_moveXY(0, 0);
    for (int i = 1; i < LINE_MAX * COLUMN_MAX *2; i += 2)
    {
        //只设置颜色
        *(current_cursor) = 0;
        *(current_cursor + i) = 0;
    }
}

void screen_init()
{
}