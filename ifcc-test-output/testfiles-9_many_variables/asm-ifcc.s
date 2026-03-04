.globl main
main:
    pushq %rbp
    movq  %rsp, %rbp
    subq  $16, %rsp
    movl  $1, %eax
    movl  %eax, -4(%rbp)
    movl  $2, %eax
    movl  %eax, -8(%rbp)
    movl  $3, %eax
    movl  %eax, -12(%rbp)
    movl  -12(%rbp), %eax
    movl  %eax, -16(%rbp)
    movl  -16(%rbp), %eax
    movl  %eax, -4(%rbp)
    movl  -4(%rbp), %eax
    movl  %eax, -8(%rbp)
    movl  -8(%rbp), %eax
    movq  %rbp, %rsp
    popq  %rbp
    ret
