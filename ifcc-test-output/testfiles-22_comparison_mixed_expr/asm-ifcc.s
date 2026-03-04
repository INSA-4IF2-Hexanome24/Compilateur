.globl main
main:
    pushq %rbp
    movq  %rsp, %rbp
    subq  $16, %rsp
    movl  $42, %eax
    movl  %eax, -4(%rbp)
    movl  -4(%rbp), %eax
    pushq %rax
    movl  $42, %eax
    popq  %rcx
    cmpl  %eax, %ecx
    sete  %al
    movzbl %al, %eax
    movq  %rbp, %rsp
    popq  %rbp
    ret
