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
    mov r14, rbx
    add r14, r12
    mov r15, r14
    mov r14, r13
    add r14, 3
    mov r10, r14
    mov r14, rbx
    mov r11, r14
    mov r14, r12
    add r14, r11
    mov r8, r14
    mov r14, rbx
    add r14, 3
    mov r9, r14
    mov r14, r12
    imul r14, 2
    mov rcx, r14
    mov r14, r10
    add r14, r8
    mov rdx, r14
    mov r14, r13
    imul r14, r12
    mov rdi, r14
    mov r14, 10
    sub r14, r12
    mov rsi, r14
    mov r14, r8
    sub r14, rbx
    mov qword ptr [rbp - 8], r14
    mov r14, 4
    imul r14, r12
    mov qword ptr [rbp - 16], r14
    mov r14, 10
    imul r14, r13
    mov qword ptr [rbp - 24], r14
    mov r13, r15
    add r13, r13
    mov rbx, r13
    add rbx, rbx
    mov r12, rbx
    add r12, r12
    mov rbx, r10
    imul rbx, r11
    mov r13, rbx
    add r13, r8
    mov rbx, r12
    sub rbx, r13
    mov r12, r9
    add r12, rcx
    mov r13, rdx
    sub r13, rdi
    mov r14, r12
    imul r14, r13
    mov r12, rsi
    add r12, qword ptr [rbp - 8]
    mov r13, r12
    add r13, qword ptr [rbp - 16]
    mov r12, r13
    sub r12, qword ptr [rbp - 24]
    mov r13, r14
    imul r13, r12
    mov r12, r13
    add r12, r12
    mov r13, rbx
    add r13, r12
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

.text
    .globl _main
    .type _main, @function
  _main:
    .cfi_startproc
    push rbp
    mov rbp, rsp
    mov rbx, 0
    mov r12, 10
    mov rbx, 3
    add rbx, r12
    mov rax, rbx
    mov rsp, rbp
    pop rbp
    ret
    .cfi_endproc
  _main.end:
    .size _main, .-_main

