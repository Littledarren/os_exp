#include <mm.h>

#include <types.h>
#include <stddef.h>




//记录页引用。只有16M大小，是在loader.asm中定义的。当然低4M是不可用的。但是仍然存在在这个数组中。
static int page[PAGE_NUM] = {0};

void * malloc_page(int n)
{
    void * addr = NULL;
    for(int i = PAGE_NUM - 2; i >= PAGE(LOW_MEMORY); i--)
    {
        int j = i;
        while(j >= 0 && page[j] == 0 && i - j + 1 != n)
        {
            j--;
        }
        if(j >= PAGE(LOW_MEMORY) && page[j] == 0)
        {
            addr = (void *)(j<<12);
            break;
        }
    }

    for(int i = PAGE(addr); i < PAGE(addr) + n; i++)
    {
        page[i]++;
    }
    return addr;
}
//前含后不包
void free_page(void * begAddr, void * endAddr)
{
    if(endAddr <=  begAddr || begAddr <= LOW_MEMORY)
    {
        return;
    }
    int begPage = PAGE(begAddr);
    int endPage = PAGE(endAddr);
    for(int i = begPage; i < endPage; i++)
    {
        page[i] = page[i] == 0?0:(page[i] - 1);
    }
}


void memInit()
{
    //??
}