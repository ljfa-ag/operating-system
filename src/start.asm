extern kernel_main
global _start

MB_MAGIC    equ 0x1badb002
MB_FLAGS    equ 0x0
MB_CHECKSUM equ -(MB_MAGIC + MB_FLAGS)

;Multiboot header
section multiboot
    align 4
    dd MB_MAGIC
    dd MB_FLAGS
    dd MB_CHECKSUM

section .text

_start:
    mov esp, kernel_stack
    call kernel_main
    cli
    hlt

;Kernel stack
section .bss
    resb 8192
kernel_stack:
