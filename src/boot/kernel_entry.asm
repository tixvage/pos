MAGIC 		equ		0x1badb002
FLAGS 		equ		(1<<0 | 1<<1 | 1<<2)
CHECKSUM 	equ		-(MAGIC + FLAGS)

section .multiboot
align 4
    dd MAGIC
    dd FLAGS
    dd CHECKSUM
	dd 0, 0, 0, 0, 0
	dd 0
	dd 640, 480, 32

section .text
[extern kernel_main]
global _start

[bits 32]
_start:
	mov esp, kernel_stack
    push ebx
    call kernel_main

_stop:
    cli
    hlt
    jmp _stop


section .bss
	align 2*1024*1024; # 2 MiB
kernel_stack:


