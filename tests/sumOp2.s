.intel_syntax noprefix
.text
    .globl _inter
    .type _inter, @function
  _inter:
    .cfi_startproc
    push rbp
    mov rbp, rsp
    mov rbx, 10
    mov r12, 5
    mov r13, 4
    mov rbx, 4
    mov r14, rbx
    add r14, r12
    mov r15, r14
    mov rbx, 5
    mov r13, r15
    add r13, r13
    mov rbx, r13
    add rbx, rbx
    mov r12, rbx
    add r12, r12
    mov rax, r12
    mov rsp, rbp
    pop rbp
    ret
    .cfi_endproc

  _inter.end:
    .size _inter, .-_inter

