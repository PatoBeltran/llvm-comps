.intel_syntax noprefix
.text
    .globl _sum
    .type _sum, @function
  _sum:
    .cfi_startproc
    push rbp
    mov rbp, rsp
    mov rbp, rsp
    add rbp, 3
    mov rax, rbp
    add rax, 7
    mov rbx, rax
    add rbx, 43
    mov rax, rbx
    mov rsp, rbp
    pop rbp
    ret
    .cfi_endproc
  _sum.end:
    .size _sum, .-_sum

