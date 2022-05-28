


all: kernel.bin

kernel.bin: start.o
	ld -T scripts/link.lds -o $@ $^

start.o:  boot/start.asm
	nasm -f elf -o $@ $^
