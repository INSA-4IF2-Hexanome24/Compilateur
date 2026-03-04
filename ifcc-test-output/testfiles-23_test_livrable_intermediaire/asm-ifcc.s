.globl main
main:
    pushq %rbp
    movq  %rsp, %rbp
    subq  $16, %rsp
    movl  $17, %eax
    movl  %eax, -4(%rbp)
    movl  $42, %eax
    movl  %eax, -8(%rbp)
    movl  -4(%rbp), %eax
    pushq %rax
    movl  -4(%rbp), %eax
    popq  %rcx
    imull %ecx, %eax
    pushq %rax
    movl  -8(%rbp), %eax
    pushq %rax
    movl  -8(%rbp), %eax
    popq  %rcx
    imull %ecx, %eax
    popq  %rcx
    addl  %ecx, %eax
    pushq %rax
    movl  $1, %eax
    popq  %rcx
    addl  %ecx, %eax
    movl  %eax, -12(%rbp)
    movl  -12(%rbp), %eax
    movq  %rbp, %rsp
    popq  %rbp
    ret
