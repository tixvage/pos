ENTRY(_start)
OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386:i386)
SECTIONS {
    . = 0xC0100000;
    KERNEL_BEGIN = .;
    KERNEL_BEGIN_PHYS = . - 0xC0000000;

	.text : AT(ADDR(.text) - 0xC0000000)
	{
		*(.multiboot)
		*(.text)
	}

	.rodata ALIGN(4K) : AT(ADDR(.rodata) - 0xC0000000)
	{
		*(.rodata)
	}

	.data ALIGN(4K) : AT(ADDR(.data) - 0xC0000000)
	{
		*(.data)
	}

	.bss ALIGN(4K) : AT(ADDR(.bss) - 0xC0000000)
	{
       _sbss = .;
		*(COMMON)
		*(.bss)
       _ebss = .;
	}

    KERNEL_END = .;
    KERNEL_END_PHYS = . - 0xC0000000;
    KERNEL_SIZE = KERNEL_END - KERNEL_BEGIN;
}
