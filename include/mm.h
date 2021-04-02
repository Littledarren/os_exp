#ifndef MM_H
#define MM_H

//分页管理，其实并不是
//
//  简单来说，只是启用了分页机制，实际上因为更改页表需要刷新TLB，有些麻烦
//  而且由于这个分页也是直接映射，有和没有是一样的。因为只是搭建一个框架和通用的接口，所以尽可能简化
//  当然还是按照一页来管理的。嗯。不过没有安全性可言
//  内存管理。。

//低4M内存不管
#define LOW_MEMORY ((void*)(4 * 1024 * 1024))
#define PAGE_NUM ((16 * 1024 * 1024)>>12)
//4K  -->  12位
#define PAGE(addr) \
    (((int)addr)>>12)



//请求n段连续4K的内存
void * malloc_page(int n);
//释放一段内存
void free_page(void * begAddr, void * endAddr);

void memInit();

#endif