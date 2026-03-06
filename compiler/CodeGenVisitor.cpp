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

antlrcpp::Any CodeGenVisitor::visitDecl_stmt(ifccParser::Decl_stmtContext *ctx)
{
    // S'il y a une expression d'initialisation
    if (ctx->expr()) {
        visit(ctx->expr());   // résultat dans %eax
        int idx = symbolTable[ctx->VAR()->getText()];
        std::cout << "    movl  %eax, " << idx << "(%rbp)\n";
    }
    // Sinon : variable non initialisée, on ne génère rien
    return 0;
}