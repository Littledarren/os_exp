
INCLUDE_FLAG = -I include/

GCC = gcc $(INCLUDE_FLAG) -c -O0 -Wall  -nostdinc -fno-builtin -fno-stack-protector -funsigned-char \
		  -m32 


#最重要的部分
#依赖部分
#驱动部分
#库文件
#用户程序
OBJS = 	temp/loader.o temp/os_main.o \
		temp/init.o temp/idt.o temp/gdt.o temp/irq.o temp/sched.o temp/mm.o \
		temp/kbd.o temp/screen.o \
		temp/asm.o temp/string.o  temp/debug.o temp/stdio.o  \
		temp/prog.o temp/shell.o

default :
	make clean
	mkdir temp || pwd
	make run

#生成软盘镜像
doc/floppy.img : boot/floppy.asm temp/IPL.bin temp/kernel
	nasm -I boot/ -f bin  boot/floppy.asm -o doc/floppy.img

#引导
temp/IPL.bin : boot/bootloader.asm boot/setup.asm
	nasm -I boot/ -f bin boot/bootloader.asm -o $@
#内核
temp/kernel : script/linkScript.ld $(OBJS) 
	ld -T script/linkScript.ld -melf_i386 -static -o temp/kernel.elf $(OBJS) -M > SYSTEM.map
	objcopy -O binary temp/kernel.elf temp/kernel

#obj
temp/loader.o : kernel/loader.asm kernel/asm.asm
	nasm -I kernel/ -f elf32  kernel/loader.asm -o temp/loader.o

temp/%.o : */%.c
	$(GCC) -c $^ -o $@  

temp/kbd.o temp/screen.o : drv/kbd.c drv/screen.c include/drv/kbd.h include/drv/screen.h
	$(GCC) -c drv/kbd.c -o temp/kbd.o
	$(GCC) -c drv/screen.c -o temp/screen.o

temp/mm.o : mm/mm.c include/mm.h
	$(GCC) -c mm/mm.c -o $@  

# temp/idt.o :

# temp/gdt.o :

# temp/irq.o :

# temp/main.o : kernel/os_main.c 
	
# temp/init.o : kernel/init.c 

# # 	$(GCC) -c $^ -o $@  
# # 	$(GCC) -S $^ -o lst/$*.s
# temp/interrupt_functions.o : kernel/interrupt_functions.c
# # 	$(GCC) -c $^ -o $@  
# # 	$(GCC) -S $^ -o lst/$*.s
# temp/prog.o : kernel/prog.c
# # 	$(GCC) -c $^ -o $@  
# # 	$(GCC) -S $^ -o lst/$*.s
# #lib
# temp/debug.o : lib/debug.c
# # 	$(GCC) -c $^ -o $@  
# # 	$(GCC) -S $^ -o lst/$*.s
# temp/io.o : lib/io.c
# # 	$(GCC) -c $^ -o $@  
# # 	$(GCC) -S $^ -o lst/$*.s
# temp/string.o : lib/string.c
# # 	$(GCC) -c $^ -o $@  
# # 	$(GCC) -S $^ -o lst/$*.s
# temp/stdio.o : lib/stdio.c
# # 	$(GCC) -c $^ -o $@  
# # 	$(GCC) -S $^ -o lst/$*.s
# temp/kbd.o : lib/kbd.c
# # 	$(GCC) -c $^ -o $@  
# # 	$(GCC) -S $^ -o lst/$*.s


run : doc/floppy.img
	bochs -f script/bochsrc.bxrc -q

clean :
	rm -f temp/*
	# rm -f lst/*
	rm -f doc/floppy.img