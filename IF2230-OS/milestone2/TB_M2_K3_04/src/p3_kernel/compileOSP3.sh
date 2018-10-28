dd if=bootload of=floppya.img bs=512 count=1 conv=notrunc
dd if=map.img of=floppya.img bs=512 count=1 seek=1 conv=notrunc
dd if=dir.img of=floppya.img bs=512 count=1 seek=2 conv=notrunc
bcc -ansi -c -o p3_kernel.o p3_kernel.c
as86 p3_kernel.asm -o p3_kernel_asm.o
ld86 -o p3_kernel -d p3_kernel.o p3_kernel_asm.o
dd if=p3_kernel of=floppya.img bs=512 conv=notrunc seek=3
dd if=message.txt of=floppya.img bs=512 count=1 seek=30 conv=notrunc
bcc -ansi -c -o shell.o shell.c
as86 lib.asm -o lib_asm.o
ld86 -o shell -d shell.o lib_asm.o
bcc -ansi -c -o uprog.o uprog.c
ld86 -o uprog -d uprog.o lib.o
./loadFile uprog
./loadFile shell
./loadFile message.txt
