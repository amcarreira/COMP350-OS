bcc -ansi -c -o kernel_c.o kernel.c
as86 kernel.asm -o kernel_asm.o
ld86 -o kernel -d kernel_c.o kernel_asm.o
dd if=kernel of=floppya.img bs=512 conv=notrunc seek=3
dd if=map.img of=floppya.img bs=512 count=1 seek=1 conv=notrunc
dd if=dir.img of=floppya.img bs=512 count=1 seek=2 conv=notrunc
