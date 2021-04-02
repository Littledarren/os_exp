#include <string.h>

//以下三个函数来自于/arch/x86/boot/compressed/string.c

static void *__memcpy(void *dest, const void *src, size_t n)
{
    // //汇编版本，// 位于 /arch/x86/boot/compressed/string.c  没有编译通过
    // int d0, d1, d2;

    // asm (

    // 	"rep ; movsl\n\t"

    // 	"movl %4,%%ecx\n\t"

    // 	"rep ; movsb\n\t"

    // 	: "=&c" (d0), "=&D" (d1), "=&S" (d2)

    // 	: "0" (n >> 2), "g" (n & 3), "1" (dest), "2" (src)

    // 	: "memory");

    // return dest;

    // //先32位一次转移,通过汇编写出的c
    // int dw = n / 4;
    // int left = n % 4;
    // int *destPtr = (int *)dest;
    // const int *srcPtr = (const int *)src;
    // while (dw--)
    // {
    //     *destPtr = *srcPtr;
    //     destPtr++;
    //     srcPtr++;
    // }
    // //再8位一转移
    // char *destPtr_byte = (char *)destPtr;
    // const char *srcPtr_byte = (const char *)src;
    // while (left--)
    // {
    //     *destPtr_byte = *srcPtr_byte;
    //     destPtr_byte++;
    //     srcPtr_byte++;
    // }

    int i = 0;
    unsigned char *d = dest;
    const unsigned char *s = src;
    int size = n;
    while(i < size)
    {
        d[i] = s[i];  
        i++;
    }
    return dest;
}

// 位于 /arch/x86/boot/compressed/string.c
void *memmove(void *dest, const void *src, size_t n)
{
    unsigned char *d = dest;
    const unsigned char *s = src;
    if (d <= s)
        return __memcpy(dest, src, n);
    int size = n;
    for(int i = size - 1; i >=0 ; i--)
    {
        d[i] = s[i];
    }

    return dest;
}

// 位于 /arch/x86/boot/compressed/string.c
void *memcpy(void *dest, const void *src, size_t n)
{
    int diff = (int)dest - (int)src;
    if (dest > src && diff < (int)n)
    {
        return memmove(dest, src, n);
    }

    return __memcpy(dest, src, n);
}
void *memset(void *dest, uint8_t num, size_t n)
{
    for(size_t i = 0; i < n; i++)
    {
        *((uint8_t*)dest + i) = num;
    }
    return dest;
}
int strlen(const char *str)
{
    int count = 0;
    while (*(str + count) != 0)
    {
        count++;
    }
    return count;
}

int strcmp(const char *left, const char *right)
{
    if (strlen(left) != strlen(right))
    {
        return strlen(left) - strlen(right);
    }
    else
    {
        while (*left++ == *right++ && *left != 0)
            ;

        return *left - *right;
    }
}

void strip(char *str)
{
    int i = 0;
    int j = strlen(str) - 1;
    while (str[i++] == ' ')
        ;
    while (str[j--] == ' ')
        ;
    i--;
    j++;
    int k = 0;
    while (i <= j)
    {
        char temp = str[i++];
        str[k++] = temp;
    }
    str[i] = 0;
}

int strncmp(char *left, char *right, int n)
{
    if (n > strlen(left) || n > strlen(right) || n < 0)
    {
        return strcmp(left, right);
    }
    else
    {
        char left_end = left[n];
        char right_end = right[n];
        left[n] = 0;
        right[n] = 0;
        int result = strcmp(left, right);
        left[n] = left_end;
        right[n] = right_end;
        return result;
    }
}


int isdigit(char ch)
{
    return (ch >= '0' && ch <= '9');
}
int isalpha(char ch)
{
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}