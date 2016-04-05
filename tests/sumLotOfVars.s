.intel_syntax noprefix
.text
    .globl _inter
    .type _inter, @function
  _inter:
    .cfi_startproc
    push rbp
    mov rbp, rsp
    mov rsp, 10
    mov rbp, 5
    mov rax, 4
    mov rbx, rsp
    add rbx, rbp
    mov rcx, rbx
    mov rdx, rax
    add rdx, 3
    mov rdi, rdx
    mov r8, rsp
    mov rsi, r8
    mov r9, rbp
    add r9, rsi
    mov r10, r9
    mov r11, rsp
    add r11, 3
    mov r12, r11
    mov r12, rbp
    imul r12, 2
    mov qword ptr [rbp - 16], r12
    mov qword ptr [rbp - 24], rdi
    idiv qword ptr [rbp - 24], r10
    mov qword ptr [rbp - 32], qword ptr [rbp - 24]
    mov qword ptr [rbp - 40], rax
    imul qword ptr [rbp - 40], rbp
    mov qword ptr [rbp - 48], qword ptr [rbp - 40]
    mov qword ptr [rbp - 56], 10
    mod qword ptr [rbp - 56], rbp
    mov qword ptr [rbp - 64], qword ptr [rbp - 56]
    mov qword ptr [rbp - 72], r10
    sub qword ptr [rbp - 72], rsp
    mov qword ptr [rbp - 80], qword ptr [rbp - 72]
    mov qword ptr [rbp - 88], 4
    imul qword ptr [rbp - 88], rbp
    mov qword ptr [rbp - 96], qword ptr [rbp - 88]
    mov qword ptr [rbp - 104], 10
    idiv qword ptr [rbp - 104], rax
    mov qword ptr [rbp - 112], qword ptr [rbp - 104]
    mov qword ptr [rbp - 120], rcx
    add qword ptr [rbp - 120], rax
    mov r13, qword ptr [rbp - 120]
    add r13, rsp
    mov r14, r13
    add r14, rbp
    mov rsp, rdi
    imul rsp, rsi
    mov rbp, rsp
    idiv rbp, r10
    mov rax, r14
    sub rax, rbp
    mov rbx, qword ptr [rbp - 8]
    add rbx, qword ptr [rbp - 16]
    mov rcx, qword ptr [rbp - 32]
    sub rcx, qword ptr [rbp - 48]
    mov rdx, rbx
    imul rdx, rcx
    mov rdi, qword ptr [rbp - 64]
    add rdi, qword ptr [rbp - 80]
    mov rsi, rdi
    add rsi, qword ptr [rbp - 96]
    mov r8, rsi
    sub r8, qword ptr [rbp - 112]
    mov r9, rdx
    imul r9, r8
    mov r11, r9
    add r11, r10
    mov r12, rax
    add r12, r11
    mov r14, r12
    add r14, r13
    mov rbp, r14
    add rbp, rsp
    mov rax, rbp
    mov rsp, rbp
    pop rbp
    ret
    .cfi_endproc
  _inter.end:
    .size _inter, .-_inter

