MAGIC 		equ		0x1badb002
FLAGS 		equ		(1<<0 | 1<<1 | 1<<2)
CHECKSUM 	equ		-(MAGIC + FLAGS)

KERNEL_VIRTUAL_BASE equ 0xC0000000
KERNEL_PAGE_NUMBER  equ (KERNEL_VIRTUAL_BASE >> 22)

global boot_page_directory

section .data
align 0x1000
boot_page_directory:
	dd 0x00000083
	times (KERNEL_PAGE_NUMBER - 1) dd 0
	dd 0x00000083
	times (1024 - KERNEL_PAGE_NUMBER - 1) dd 0

section .multiboot
align 4
    dd MAGIC
    dd FLAGS
    dd CHECKSUM
	dd 0, 0, 0, 0, 0
	dd 0
	dd 1024, 768, 32

[extern kernel_main]

[bits 32]
section .text
global _start
_start:
    mov ecx, (boot_page_directory - KERNEL_VIRTUAL_BASE)
    mov cr3, ecx
 
    mov ecx, cr4
    or ecx, (1 << 4)
    mov cr4, ecx
 
    mov ecx, cr0
    or ecx, (1 << 31)
    mov cr0, ecx

    lea ecx, [.start_higher_half] ; Far jump
    jmp ecx

.start_higher_half:
    mov dword [boot_page_directory], 0
    invlpg [0]
    
	mov esp, stack_top

    push ebx

	;enable sse
	;TODO: check if sse is available
    mov eax, cr0
    and ax, ~(1 << 2)
    or ax, (1 << 1)
    mov cr0, eax
    mov eax, cr4
    or ax, (3 << 9)
    mov cr4, eax

	call kernel_main
	cli

_old_start:
	mov esp, stack_top
    push ebx
    call kernel_main

_stop:
    hlt
    jmp _stop


;section .bss
;	align 2*1024*1024; # 2 MiB
;kernel_stack:
section .bss
align 16
stack_bottom:
resb 0x4000 ; 16 KiB
stack_top:
