nasm bootload.asm

dd if=/dev/zero of=floppya.img bs=512 count=2880
dd if=bootload of=floppya.img bs=512 count=1 conv=notrunc
dd if=map.img of=floppya.img bs=512 count=1 seek=1 conv=notrunc
dd if=dir.img of=floppya.img bs=512 count=1 seek=2 conv=notrunc

gcc -o loadFile loadFile.c
bcc -ansi -c -o pcb.o pcb.c
bcc -ansi -c -o kernel.o kernel.c
as86 kernel.asm -o kernel_asm.o

ld86 -o kernel -d kernel.o pcb.o kernel_asm.o
dd if=kernel of=floppya.img bs=512 conv=notrunc seek=3
dd if=message.txt of=floppya.img bs=512 count=1 seek=30 conv=notrunc

bcc -ansi -c -o shell.o shell.c
as86 lib.asm -o lib_asm.o
ld86 -o shell -d shell.o lib_asm.o

bcc -ansi -c -o hello.o hello.c
ld86 -o hello -d hello.o lib_asm.o

bcc -ansi -c -o world.o world.c
ld86 -o world -d world.o lib_asm.o

bcc -ansi -c -o stima.o stima.c
ld86 -o stima -d stima.o lib_asm.o

bcc -ansi -c -o oop.o oop.c
ld86 -o oop -d oop.o lib_asm.o

bcc -ansi -c -o probst.o probst.c
ld86 -o probst -d probst.o lib_asm.o

bcc -ansi -c -o os.o os.c
ld86 -o os -d os.o lib_asm.o

./loadFile stima
./loadFile oop
./loadFile probst
./loadFile os
./loadFile hello
./loadFile world
./loadFile shell
./loadFile message.txt
