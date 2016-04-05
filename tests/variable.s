.intel_syntax noprefix
.text
    .globl _inter
    .type _inter, @function
  _inter:
    .cfi_startproc
    push rbp
    mov rbp, rsp
    mov r14, 10
    mov rax, r14
    mov rsp, rbp
    pop rbp
    ret
    .cfi_endproc

  _inter.end:
    .size _inter, .-_inter

