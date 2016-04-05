.intel_syntax noprefix
.text
    .globl _sum
    .type _sum, @function
  _sum:
    .cfi_startproc
    push rbp
    mov rbp, rsp
    mov rbx, rbx
    add rbx, 3
    mov r14, rbx
    add r14, 7
    mov rbx, r14
    add rbx, 43
    mov rax, rbx
    mov rsp, rbp
    pop rbp
    ret
    .cfi_endproc

  _sum.end:
    .size _sum, .-_sum

