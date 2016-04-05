.intel_syntax noprefix
.text
    .globl _inter
    .type _inter, @function
  _inter:
    .cfi_startproc
    push rbp
    mov rbp, rsp
    mov rbx, 10
    mov r15, 5
    mov r12, r15
    mov rbx, r12
    mov rax, r15
    mov rsp, rbp
    pop rbp
    ret
    .cfi_endproc

  _inter.end:
    .size _inter, .-_inter

