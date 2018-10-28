dd if=bootload of=floppya.img bs=512 count=1 conv=notrunc
dd if=map.img of=floppya.img bs=512 count=1 seek=1 conv=notrunc
dd if=dir.img of=floppya.img bs=512 count=1 seek=2 conv=notrunc
bcc -ansi -c -o p2_kernel.o p2_kernel.c
as86 lib.asm -o lib_asm.o
ld86 -o p2_kernel -d p2_kernel.o lib_asm.o
./loadFile p2_kernel
