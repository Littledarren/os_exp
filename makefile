
CFLAGS=-Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -m32
INCLUDE_FLAG= -I ./include

CC=gcc $(CFLAGS) $(INCLUDE_FLAG)

all: kernel.bin

test: main.out
	./main.out

kernel.bin: start.o main.o
	ld -T scripts/link.lds -o $@ $^

start.o:  boot/start.asm
	nasm -f elf -o $@ $^
main.o: kernel/main.c
	$(CC) -c -o $@ $^
main.out: kernel/main.c kernel/test.c
	$(CC) -o $@ $^
	
	
