C_SOURCES = $(wildcard src/*.c)
HEADERS = $(wildcard src/*.h)
OBJ = ${C_SOURCES:.c=.o src/interrupt.o src/boot/kernel_entry.o src/boot/gdt.o src/sse.o}

CC = i386-elf-gcc
CFLAGS = -g -march=i686 -O2

kernel.bin: ${OBJ}
	ld -melf_i386 -T linker.ld -o $@ $^

mykernel.iso: kernel.bin
	mkdir -p iso/boot/grub
	cp kernel.bin iso/boot/kernel.bin
	echo 'set timeout=0'                      > iso/boot/grub/grub.cfg
	echo 'set default=0'                     >> iso/boot/grub/grub.cfg
	echo ''                                  >> iso/boot/grub/grub.cfg
	echo 'menuentry "My Operating System" {' >> iso/boot/grub/grub.cfg
	echo '  multiboot /boot/kernel.bin'    >> iso/boot/grub/grub.cfg
	echo '  boot'                            >> iso/boot/grub/grub.cfg
	echo '}'                                 >> iso/boot/grub/grub.cfg
	grub-mkrescue --output=kernel.iso iso
	rm -rf iso

vbox: mykernel.iso
	VirtualBoxVM.exe --startvm 'SusOS'

qemu: mykernel.iso
	qemu-system-i386 -vga std -m 256M -cdrom kernel.iso

%.o: %.c ${HEADERS}
	${CC} ${CFLAGS} -ffreestanding -c $< -o $@

%.o: %.asm
	nasm $< -f elf32 -o $@

#TODO: proper cleanup
clean:
	rm -rf *.bin *.dis *.o os-image.bin *.elf
	rm -rf src/*.o src/boot/*.bin src/boot/*.o
