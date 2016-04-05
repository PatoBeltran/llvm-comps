.intel_syntax noprefix
.text
    .globl _inter
    .type _inter, @function
  _inter:
    .cfi_startproc
    push rbp
    mov rbp, rsp
    mov rbx, 5
    mov r12, rbx
    mov r15, r12
    mov rax, rbx
    mov rsp, rbp
    pop rbp
    ret
    .cfi_endproc

  _inter.end:
    .size _inter, .-_inter

