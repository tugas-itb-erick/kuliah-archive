as86 lib.asm -o lib.o
bcc -ansi -c -o uprog.o uprog.c
ld86 -o uprog -d uprog.o lib.o
./loadFile uprog
