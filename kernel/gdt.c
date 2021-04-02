#include <asm.h>
#include <sched.h>
#include <stddef.h>
#include <types.h>
#include <kernel_head.h>

//GDT start 
const int GDTAddress = 0x270000;
// const int LDTAddress = 0x300000;


static int gdt_used[0xffff + 1] = {0};
//GDT指针
static struct 
{
    uint16_t limit;
    uint32_t address;
}__attribute__ ((packed)) GDT_Ptr;

//===================================================================================
void setSegDesc(SegmentDescriper *GDT, unsigned int limit, int base, int ar)
{
	GDT->limit_low    = limit & 0xffff;
	GDT->base_low     = base & 0xffff;
	GDT->base_mid     = (base >> 16) & 0xff;
	GDT->access_right = ar & 0xff;
	GDT->limit_high   = (limit >> 16) & 0xf;
    GDT->flags = (ar >> 8) & 0xf;
	GDT->base_high    = (base >> 24) & 0xff;
}

void init_GDT()
{
    SegmentDescriper * GDT = (SegmentDescriper *) GDTAddress;
    for(int i = 0; i < 8192; i++)
    {
        setSegDesc(GDT + i, 0, 0, 0); //NULL
    }
    gdt_used[0] = 1;
    gdt_used[1] = 1;
    gdt_used[2] = 1;
    //三个保留
    setSegDesc(GDT + 1, 0xfffff, 0, 0b110010011010); //CODE
    setSegDesc(GDT + 2, 0xfffff, 0, 0b110010010010); //DATA
    //LDT
    // setSegDesc(GDT + 3, 0xfffff, LDTAddress, 0b110010010010);
    // asm volatile("lldtl %0" : : "m" ((uint16_t)(3<<3)))
    GDT_Ptr.limit = 0xffff;
    GDT_Ptr.address = (uint32_t)GDTAddress;
    asm volatile("lgdtl %0" : : "m" (GDT_Ptr));
}



int alloc_gdt()
{
    for(int i = 1; i < 0xffff; i++)
    {
        if(gdt_used[i] == 0)
        {
            gdt_used[i] = 1;
            return i;
        }
    }
    return -1;
}
void free_gdt(int gdt)
{
    if(gdt <= 2)
    {
        return;
    }
    gdt_used[gdt] = 0;
}
