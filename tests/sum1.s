.intel_syntax noprefix
.text
    .globl _inter
    .type _inter, @function
  _inter:
    .cfi_startproc
    push rbp
    mov rbp, rsp
    mov rbp, 1
    add rbp, rsp
    mov rax, rbp
    add rax, 4
    mov rcx, 3
    add rcx, rbx
    mov rdx, rcx
    add rdx, 6
    mov rdi, rax
    add rdi, rdx
    mov rax, rdi
    mov rsp, rbp
    pop rbp
    ret
    .cfi_endproc
  _inter.end:
    .size _inter, .-_inter

