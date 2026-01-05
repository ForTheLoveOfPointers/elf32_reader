bits 64

section .data
    message: times 512 db 0
    buf_len: dw $ -message
    file_name: db "text.txt"

section .text
    global _start

_start:
    ;Open file
    mov rax, 2
    mov rdi, file_name
    mov rsi, 0
    mov rdx, 0644
    syscall
    push rax ;Push fd into stack

    ;Read file
    mov rax, 0
    pop rdi ;Get fd into rdi
    mov rsi, message
    mov rdx, buf_len
    syscall
    push rdi ; Save fd and then read_count
    push rax

    ;Print file contents into stdout
    mov rax, 1
    mov rdi, 1
    mov rsi, message
    pop rdx ;Get read count into message length
    syscall

    ;Close file
    mov rax, 3
    pop rdi
    syscall

    mov rax, 60
    xor rdi, rdi
    syscall
