#include "CodeGenVisitor.h"

antlrcpp::Any CodeGenVisitor::visitProg(ifccParser::ProgContext *ctx)
{
    // Compute total stack space needed (one slot per variable, rounded to 16)
    int frameSize = symbolTable.size() * 4;
    // Align to 16 bytes for the ABI
    if (frameSize % 16 != 0) frameSize += 16 - (frameSize % 16);

#ifdef __APPLE__
    std::cout << ".globl _main\n";
    std::cout << "_main:\n";
#else
    std::cout << ".globl main\n";
    std::cout << "main:\n";
#endif

    // Prologue
    std::cout << "    pushq %rbp\n";
    std::cout << "    movq  %rsp, %rbp\n";
    if (frameSize > 0)
        std::cout << "    subq  $" << frameSize << ", %rsp\n";

    visitChildren(ctx);

    // Epilogue
    std::cout << "    movq  %rbp, %rsp\n";
    std::cout << "    popq  %rbp\n";
    std::cout << "    ret\n";

    return 0;
}

// Declaration with optional initialization: int a; or int a=42;
antlrcpp::Any CodeGenVisitor::visitDecl_stmt(ifccParser::Decl_stmtContext *ctx)
{
    std::string name = ctx->VAR()->getText();
    
    // If there is initialization (int a=42;), we evalue expr et on garde la variable
    if (ctx->expr()) {
        visit(ctx->expr());  // resultat dans %eax
        int idx = symbolTable[name];
        std::cout << "    movl  %eax, " << idx << "(%rbp)\n";
    }
    
    return 0;
}


// x = expr  →  evaluate expr into %eax, then store at x's slot
antlrcpp::Any CodeGenVisitor::visitAssign_stmt(ifccParser::Assign_stmtContext *ctx)
{
    visit(ctx->expr());   // result ends up in %eax
    std::string name = ctx->VAR()->getText();
    int idx = symbolTable[name];
    std::cout << "    movl  %eax, " << idx << "(%rbp)\n";
    return 0;
}

// return expr  →  evaluate expr into %eax (already the return register)
antlrcpp::Any CodeGenVisitor::visitReturn_stmt(ifccParser::Return_stmtContext *ctx)
{
    visit(ctx->expr());
    return 0;
}

// Leaf: integer constant
antlrcpp::Any CodeGenVisitor::visitConstExpr(ifccParser::ConstExprContext *ctx)
{
    int val = stoi(ctx->CONST()->getText());
    std::cout << "    movl  $" << val << ", %eax\n";
    return 0;
}

// Leaf: variable read
antlrcpp::Any CodeGenVisitor::visitVarExpr(ifccParser::VarExprContext *ctx)
{
    std::string name = ctx->VAR()->getText();
    int idx = symbolTable[name];
    std::cout << "    movl  " << idx << "(%rbp), %eax\n";
    return 0;
}

// Comparison: expr == expr
antlrcpp::Any CodeGenVisitor::visitEqExpr(ifccParser::EqExprContext *ctx)
{
    visit(ctx->expr(0));          // left expr → %eax
    std::cout << "    movl  %eax, %ecx\n";  // save left in %ecx
    visit(ctx->expr(1));          // right expr → %eax
    std::cout << "    cmpl  %eax, %ecx\n";  // compare left (ecx) with right (eax)
    std::cout << "    sete  %al\n";         // set %al = 1 if equal
    std::cout << "    movzbl %al, %eax\n";  // zero-extend %al to %eax
    return 0;
}

// Comparison: expr != expr
antlrcpp::Any CodeGenVisitor::visitNeExpr(ifccParser::NeExprContext *ctx)
{
    visit(ctx->expr(0));          // left expr → %eax
    std::cout << "    movl  %eax, %ecx\n";  // save left in %ecx
    visit(ctx->expr(1));          // right expr → %eax
    std::cout << "    cmpl  %eax, %ecx\n";  // compare left (ecx) with right (eax)
    std::cout << "    setne %al\n";         // set %al = 1 if not equal
    std::cout << "    movzbl %al, %eax\n";  // zero-extend %al to %eax
    return 0;
}

// Comparison: expr < expr
antlrcpp::Any CodeGenVisitor::visitLtExpr(ifccParser::LtExprContext *ctx)
{
    visit(ctx->expr(0));          // left expr → %eax
    std::cout << "    movl  %eax, %ecx\n";  // save left in %ecx
    visit(ctx->expr(1));          // right expr → %eax
    std::cout << "    cmpl  %eax, %ecx\n";  // compare left (ecx) with right (eax)
    std::cout << "    setl  %al\n";         // set %al = 1 if left < right
    std::cout << "    movzbl %al, %eax\n";  // zero-extend %al to %eax
    return 0;
}

// Comparison: expr > expr
antlrcpp::Any CodeGenVisitor::visitGtExpr(ifccParser::GtExprContext *ctx)
{
    visit(ctx->expr(0));          // left expr → %eax
    std::cout << "    movl  %eax, %ecx\n";  // save left in %ecx
    visit(ctx->expr(1));          // right expr → %eax
    std::cout << "    cmpl  %eax, %ecx\n";  // compare left (ecx) with right (eax)
    std::cout << "    setg  %al\n";         // set %al = 1 if left > right
    std::cout << "    movzbl %al, %eax\n";  // zero-extend %al to %eax
    return 0;
}

// Parenthesized expression
antlrcpp::Any CodeGenVisitor::visitParenExpr(ifccParser::ParenExprContext *ctx)
{
    visit(ctx->expr());
    return 0;
}