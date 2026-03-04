.globl main
main:
    pushq %rbp
    movq  %rsp, %rbp
    subq  $16, %rsp
    movl  $7, %eax
    movl  %eax, -4(%rbp)
    movl  -4(%rbp), %eax
    movl  %eax, -8(%rbp)
    movl  -8(%rbp), %eax
    movq  %rbp, %rsp
    popq  %rbp
    ret
