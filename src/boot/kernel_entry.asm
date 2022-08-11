MAGIC 		equ		0x1badb002
FLAGS 		equ		(1<<0 | 1<<1)
CHECKSUM 	equ		-(MAGIC + FLAGS)

section .multiboot
    dd MAGIC
    dd FLAGS
    dd CHECKSUM


section .text
[extern kernel_main]
global _start

[bits 32]
_start:
	mov esp, kernel_stack
	push eax
    push ebx
    call kernel_main

_stop:
    cli
    hlt
    jmp _stop


section .bss
	align 2*1024*1024; # 2 MiB
kernel_stack:


