bcc -ansi -c -o shell_c.o shell.c
as86 lib.asm -o lib_asm.o
ld86 -o shell -d shell_c.o lib_asm.o
dd if=shell of=floppya.img bs=512 conv=notrunc seek=3
dd if=map.img of=floppya.img bs=512 count=1 seek=1 conv=notrunc
dd if=dir.img of=floppya.img bs=512 count=1 seek=2 conv=notrunc
