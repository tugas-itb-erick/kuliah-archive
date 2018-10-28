#!/bin/bash
bcc -ansi -c -o kernel.o kernel.c
as86 -o kernel_asm.o kernel.asm
ld86 -o kernel -d kernel.o kernel_asm.o
dd if=kernel of=floppya.img bs=512 conv=notrunc seek=3
