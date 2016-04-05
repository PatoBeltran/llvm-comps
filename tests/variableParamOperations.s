.intel_syntax noprefix
.text
    .globl _inter
    .type _inter, @function
  _inter:
    .cfi_startproc
    push rbp
    mov rbp, rsp
    mov rbx, 1
    imul rbx, rbx
    mov r12, rbx
    add r12, 4
    mov rbx, 3
    imul rbx, rbx
    mov r13, rbx
    sub r13, 6
    mov rbx, r12
    sub rbx, r13
    mov rax, rbx
    mov rsp, rbp
    pop rbp
    ret
    .cfi_endproc

  _inter.end:
    .size _inter, .-_inter

