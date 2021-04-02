#include <asm.h>
#include <types.h>
#include <kernel_head.h>
//IDT指针
static struct 
{
    uint16_t limit;
    uint32_t address;
}__attribute__ ((packed)) IDT_Ptr;


//IDT start
const int IDTAddress = 0x26f800;


//===================================================================================
void setGateDesc(GateDescriper *IDT, int offset, int selector, int ar)
{
	IDT->offset_low   = offset & 0xffff;
	IDT->selector     = selector;
	IDT->dw_count     = (ar >> 8) & 0xff;
	IDT->access_right = ar & 0xff;
	IDT->offset_high  = (offset >> 16) & 0xffff;
	return;
}


void init_IDT()
{
    GateDescriper * IDT = (GateDescriper * )IDTAddress;
    for(int i = 0; i < 256; i++)
    {
        setGateDesc(IDT + i, 0, 0, 0);
    }

    IDT_Ptr.limit = 0x7ff;
    IDT_Ptr.address = IDTAddress;
    //这里使用了汇编，，，不是很好。
    asm volatile("lidtl %0" : : "m" (IDT_Ptr));
}
