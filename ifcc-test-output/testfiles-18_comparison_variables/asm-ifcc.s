.globl main
main:
    pushq %rbp
    movq  %rsp, %rbp
    subq  $16, %rsp
    movl  $10, %eax
    movl  %eax, -4(%rbp)
    movl  $10, %eax
    movl  %eax, -8(%rbp)
    movl  -4(%rbp), %eax
    pushq %rax
    movl  -8(%rbp), %eax
    popq  %rcx
    cmpl  %eax, %ecx
    sete  %al
    movzbl %al, %eax
    movq  %rbp, %rsp
    popq  %rbp
    ret
