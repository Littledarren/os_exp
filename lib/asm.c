#include <asm.h>
#include <types.h>

uint8_t in_byte(uint16_t port)
{
    uint8_t data;
    asm volatile(
        "inb %1, %0"
        : "=a"(data)
        : "dN"(port)
    );
    return data;
}

void out_byte(uint16_t port, uint8_t data)
{
    asm volatile(
        "outb %1, %0"
        :
        : "dN"(port), "a" (data)
    );
}

uint16_t in_word(uint16_t port)
{
    uint16_t data;
    asm volatile(
        "inw %1, %0"
        : "=a"(data)
        : "dN"(port)
    );
    return data;
}

void out_word(uint16_t port, uint16_t data)
{
    asm volatile(
        "outw %1, %0"
        :
        : "dN"(port), "a" (data)
    );
}



void cli()
{
    asm volatile(
        "cli"
    );
}

void sti()
{
    asm volatile(
        "sti"
    );
}

void hlt()
{
    asm volatile(
        "hlt"
    );
}
