.set MAGIC, 0x1BADB002 
.set FLAGS, (1<<0 | 1<<1 | 1<<2)
.set CHECKSUM, -(MAGIC + FLAGS)

.section .multiboot,"a"
.align 4
    .long MAGIC 
    .long FLAGS
    .long CHECKSUM
    .long 0 
    .long 0 
    .long 0 
    .long 0 
    .long 0 
    .long 0 
    .long 1024 
    .long 768
    .long 32

.section .text
.extern kernel_main
.global loader


loader:
    mov $kernel_stack, %esp
    push %eax
    push %ebx
    call kernel_main



_stop:
    cli
    hlt
    jmp _stop

.section .bss
.space 2*1024*1024 #2 MiB
kernel_stack:
