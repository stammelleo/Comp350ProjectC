dd if=/dev/zero of=diskc.img bs=512 count=256
nasm bootload.asm
dd if=bootload of=diskc.img conv=notrunc
bcc -c -ansi -o kernelc.o kernel.c
as86 kernel.asm -o kernelasm.o
ld86 -d -o kernel kernelc.o kernelasm.o
dd if=kernel of=diskc.img bs=512 conv=notrunc seek=3
dd if=message.txt of=diskc.img conv=notrunc seek=30
