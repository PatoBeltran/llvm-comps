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
    mov r12, 4
    mov r13, rbx
    add r13, r15
    mov r14, r13
    mov r13, r12
    add r13, 3
    mov r10, r13
    mov r13, rbx
    mov r11, r13
    mov r13, r15
    add r13, r11
    mov r9, r13
    mov r13, rbx
    add r13, 3
    mov r8, r13
    mov r13, r15
    imul r13, 2
    mov rcx, r13
    mov r13, r10
    add r13, r9
    mov rdx, r13
    mov r13, r12
    imul r13, r15
    mov rdi, r13
    mov r13, 10
    sub r13, r15
    mov rsi, r13
    mov r13, r9
    sub r13, rbx
    mov qword ptr [rbp - 8], r13
    mov r13, 4
    imul r13, r15
    mov qword ptr [rbp - 16], r13
    mov r13, 10
    imul r13, r12
    mov qword ptr [rbp - 24], r13
    mov r12, r14
    add r12, r12
    mov rbx, r12
    add rbx, rbx
    mov r15, rbx
    add r15, r15
    mov rbx, r10
    imul rbx, r11
    mov r12, rbx
    add r12, r9
    mov rbx, r15
    sub rbx, r12
    mov r15, r8
    add r15, rcx
    mov r12, rdx
    sub r12, rdi
    mov r13, r15
    imul r13, r12
    mov r15, rsi
    add r15, qword ptr [rbp - 8]
    mov r12, r15
    add r12, qword ptr [rbp - 16]
    mov r15, r12
    sub r15, qword ptr [rbp - 24]
    mov r12, r13
    imul r12, r15
    mov r15, r12
    add r15, r15
    mov r12, rbx
    add r12, r15
    mov rbx, r12
    add rbx, rbx
    mov r15, rbx
    add r15, r15
    mov rax, r15
    mov rsp, rbp
    pop rbp
    ret
    .cfi_endproc
  _inter.end:
    .size _inter, .-_inter

