.intel_syntax noprefix
.text
    .globl _inter
    .type _inter, @function
  _inter:
    .cfi_startproc
    push rbp
    mov rbp, rsp
    mov rbx, 1
    add rbx, rbx
    mov r12, rbx
    add r12, 4
    mov rbx, 3
    add rbx, rbx
    mov r15, rbx
    add r15, 6
    mov rbx, r12
    add rbx, r15
    mov rax, rbx
    mov rsp, rbp
    pop rbp
    ret
    .cfi_endproc

  _inter.end:
    .size _inter, .-_inter

