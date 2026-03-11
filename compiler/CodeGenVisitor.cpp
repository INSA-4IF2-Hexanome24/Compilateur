#include "CodeGenVisitor.h"

#include <iostream>
#include <string>

CodeGenVisitor::CodeGenVisitor()
{
    numTemps = 0;
}

CodeGenVisitor::CodeGenVisitor(const std::map<std::string, int> &table)
{
    symbolTable = table;
    numTemps = 0;
}

void CodeGenVisitor::setTempVar(int i)
{
    std::string name = "_temp" + std::to_string(i);
    int idx = symbolTable[name];
    std::cout << "    movl  %eax, " << idx << "(%rbp)\n";
}

void CodeGenVisitor::getTempVar(int i)
{
    std::string name = "_temp" + std::to_string(i);
    int idx = symbolTable[name];
    std::cout << "    movl  " << idx << "(%rbp), %eax\n";
}

antlrcpp::Any CodeGenVisitor::visitProg(ifccParser::ProgContext *ctx)
{
    int frameSize = (int)symbolTable.size() * 4 + 4 * 20;

    if (frameSize % 16 != 0)
    {
        frameSize = frameSize + (16 - frameSize % 16);
    }

#ifdef __APPLE__
    std::cout << ".globl _main\n";
    std::cout << "_main:\n";
#else
    std::cout << ".globl main\n";
    std::cout << "main:\n";
#endif

    std::cout << "    pushq %rbp\n";
    std::cout << "    movq  %rsp, %rbp\n";

    if (frameSize > 0)
    {
        std::cout << "    subq  $" << frameSize << ", %rsp\n";
    }

    for (auto s : ctx->stmt())
    {
        visit(s);
    }

    visit(ctx->return_stmt());

    std::cout << "    movq  %rbp, %rsp\n";
    std::cout << "    popq  %rbp\n";
    std::cout << "    ret\n";

    return 0;
}

antlrcpp::Any CodeGenVisitor::visitDecl_stmt(ifccParser::Decl_stmtContext *ctx)
{
    for (auto v : ctx->var_decl_list()->var_decl())
    {
        if (v->expr() != nullptr)
        {
            visit(v->expr());

            std::string name = v->VAR()->getText();
            int idx = symbolTable[name];
            std::cout << "    movl  %eax, " << idx << "(%rbp)\n";
        }
    }

    return 0;
}

antlrcpp::Any CodeGenVisitor::visitAssign_stmt(
    ifccParser::Assign_stmtContext *ctx)
{
    visit(ctx->expr());

    std::string name = ctx->VAR()->getText();
    int idx = symbolTable[name];
    std::cout << "    movl  %eax, " << idx << "(%rbp)\n";

    return 0;
}

antlrcpp::Any CodeGenVisitor::visitReturn_stmt(
    ifccParser::Return_stmtContext *ctx)
{
    visit(ctx->expr());
    return 0;
}

antlrcpp::Any CodeGenVisitor::visitMultdiv(ifccParser::MultdivContext *ctx)
{
    int t = numTemps;
    numTemps++;

    visit(ctx->expr(0));
    setTempVar(t);

    visit(ctx->expr(1));
    std::cout << "    movl  %eax, %ecx\n";
    getTempVar(t);

    std::cout << "    imull %ecx, %eax\n";
    return 0;
}

antlrcpp::Any CodeGenVisitor::visitDiv(ifccParser::DivContext *ctx)
{
    int t = numTemps;
    numTemps++;

    visit(ctx->expr(0));
    setTempVar(t);

    visit(ctx->expr(1));
    std::cout << "    movl  %eax, %ecx\n";
    getTempVar(t);

    std::cout << "    cltd\n";
    std::cout << "    idivl %ecx\n";

    return 0;
}

antlrcpp::Any CodeGenVisitor::visitMod(ifccParser::ModContext *ctx)
{
    int t = numTemps;
    numTemps++;

    visit(ctx->expr(0));
    setTempVar(t);

    visit(ctx->expr(1));
    std::cout << "    movl  %eax, %ecx\n";
    getTempVar(t);

    std::cout << "    cltd\n";
    std::cout << "    idivl %ecx\n";
    std::cout << "    movl  %edx, %eax\n";

    return 0;
}

antlrcpp::Any CodeGenVisitor::visitPlus(ifccParser::PlusContext *ctx)
{
    int t = numTemps;
    numTemps++;

    visit(ctx->expr(0));
    setTempVar(t);

    visit(ctx->expr(1));
    std::cout << "    movl  %eax, %ecx\n";
    getTempVar(t);

    std::cout << "    addl  %ecx, %eax\n";
    return 0;
}

antlrcpp::Any CodeGenVisitor::visitMinus(ifccParser::MinusContext *ctx)
{
    int t = numTemps;
    numTemps++;

    visit(ctx->expr(0));
    setTempVar(t);

    visit(ctx->expr(1));
    std::cout << "    movl  %eax, %ecx\n";
    getTempVar(t);

    std::cout << "    subl  %ecx, %eax\n";
    return 0;
}

antlrcpp::Any CodeGenVisitor::visitLt(ifccParser::LtContext *ctx)
{
    int t = numTemps;
    numTemps++;

    visit(ctx->expr(0));
    setTempVar(t);

    visit(ctx->expr(1));
    std::cout << "    movl  %eax, %ecx\n";
    getTempVar(t);

    std::cout << "    cmpl  %ecx, %eax\n";
    std::cout << "    setl  %al\n";
    std::cout << "    movzbl %al, %eax\n";

    return 0;
}

antlrcpp::Any CodeGenVisitor::visitGt(ifccParser::GtContext *ctx)
{
    int t = numTemps;
    numTemps++;

    visit(ctx->expr(0));
    setTempVar(t);

    visit(ctx->expr(1));
    std::cout << "    movl  %eax, %ecx\n";
    getTempVar(t);

    std::cout << "    cmpl  %ecx, %eax\n";
    std::cout << "    setg  %al\n";
    std::cout << "    movzbl %al, %eax\n";

    return 0;
}

antlrcpp::Any CodeGenVisitor::visitEq(ifccParser::EqContext *ctx)
{
    int t = numTemps;
    numTemps++;

    visit(ctx->expr(0));
    setTempVar(t);

    visit(ctx->expr(1));
    std::cout << "    movl  %eax, %ecx\n";
    getTempVar(t);

    std::cout << "    cmpl  %ecx, %eax\n";
    std::cout << "    sete  %al\n";
    std::cout << "    movzbl %al, %eax\n";

    return 0;
}

antlrcpp::Any CodeGenVisitor::visitNeq(ifccParser::NeqContext *ctx)
{
    int t = numTemps;
    numTemps++;

    visit(ctx->expr(0));
    setTempVar(t);

    visit(ctx->expr(1));
    std::cout << "    movl  %eax, %ecx\n";
    getTempVar(t);

    std::cout << "    cmpl  %ecx, %eax\n";
    std::cout << "    setne %al\n";
    std::cout << "    movzbl %al, %eax\n";

    return 0;
}

antlrcpp::Any CodeGenVisitor::visitBand(ifccParser::BandContext *ctx)
{
    int t = numTemps;
    numTemps++;

    visit(ctx->expr(0));
    setTempVar(t);

    visit(ctx->expr(1));
    std::cout << "    movl  %eax, %ecx\n";
    getTempVar(t);

    std::cout << "    andl  %ecx, %eax\n";
    return 0;
}

antlrcpp::Any CodeGenVisitor::visitBxor(ifccParser::BxorContext *ctx)
{
    int t = numTemps;
    numTemps++;

    visit(ctx->expr(0));
    setTempVar(t);

    visit(ctx->expr(1));
    std::cout << "    movl  %eax, %ecx\n";
    getTempVar(t);

    std::cout << "    xorl  %ecx, %eax\n";
    return 0;
}

antlrcpp::Any CodeGenVisitor::visitBor(ifccParser::BorContext *ctx)
{
    int t = numTemps;
    numTemps++;

    visit(ctx->expr(0));
    setTempVar(t);

    visit(ctx->expr(1));
    std::cout << "    movl  %eax, %ecx\n";
    getTempVar(t);

    std::cout << "    orl   %ecx, %eax\n";
    return 0;
}

antlrcpp::Any CodeGenVisitor::visitNotExpr(ifccParser::NotExprContext *ctx)
{
    visit(ctx->expr());

    std::cout << "    cmpl  $0, %eax\n";
    std::cout << "    sete  %al\n";
    std::cout << "    movzbl %al, %eax\n";

    return 0;
}

antlrcpp::Any CodeGenVisitor::visitUnaryMinus(
    ifccParser::UnaryMinusContext *ctx)
{
    visit(ctx->expr());
    std::cout << "    negl  %eax\n";
    return 0;
}

antlrcpp::Any CodeGenVisitor::visitParens(ifccParser::ParensContext *ctx)
{
    return visit(ctx->expr());
}

antlrcpp::Any CodeGenVisitor::visitConstExpr(
    ifccParser::ConstExprContext *ctx)
{
    int val = std::stoi(ctx->CONST()->getText());
    std::cout << "    movl  $" << val << ", %eax\n";
    return 0;
}

antlrcpp::Any CodeGenVisitor::visitVarExpr(ifccParser::VarExprContext *ctx)
{
    std::string name = ctx->VAR()->getText();
    int idx = symbolTable[name];
    std::cout << "    movl  " << idx << "(%rbp), %eax\n";
    return 0;
}

antlrcpp::Any CodeGenVisitor::visitIf_stmt(ifccParser::If_stmtContext *ctx)
{
    int n = labelCount++;
    std::string elseLabel = ".Lelse_" + std::to_string(n);
    std::string endLabel  = ".Lend_"  + std::to_string(n);

    visit(ctx->expr());
    std::cout << "    cmpl  $0, %eax\n";
    std::cout << "    je    " << elseLabel << "\n";

    visit(ctx->block(0)); // then
    std::cout << "    jmp   " << endLabel << "\n";

    std::cout << elseLabel << ":\n";
    if (ctx->block().size() > 1)
        visit(ctx->block(1)); // else

    std::cout << endLabel << ":\n";
    return 0;
}

antlrcpp::Any CodeGenVisitor::visitBlock(ifccParser::BlockContext *ctx)
{
    for (auto s : ctx->stmt())
        visit(s);
    return 0;
}