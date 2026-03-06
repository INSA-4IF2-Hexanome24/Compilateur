#include "CodeGenVisitor.h"

#include <iostream>

static void pushRax()
{
    std::cout << "    pushq %rax\n";
}

static void popRcx()
{
    std::cout << "    popq  %rcx\n";
}

antlrcpp::Any CodeGenVisitor::visitProg(ifccParser::ProgContext *ctx)
{
    int frameSize = (int)symbolTable.size() * 4;
    if (frameSize % 16 != 0) {
        frameSize += 16 - (frameSize % 16);
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
    if (frameSize > 0) {
        std::cout << "    subq  $" << frameSize << ", %rsp\n";
    }

    visitChildren(ctx);

    std::cout << "    movq  %rbp, %rsp\n";
    std::cout << "    popq  %rbp\n";
    std::cout << "    ret\n";

    return 0;
}

// Declaration with optional initialization: int a; or int a = expr;
antlrcpp::Any CodeGenVisitor::visitDecl_stmt(ifccParser::Decl_stmtContext *ctx)
{
    auto varListCtx = ctx->var_decl_list();

    for (auto varDeclCtx : varListCtx->var_decl()) {

        // Si la variable a une initialisation
        if (varDeclCtx->expr() != nullptr) {
            visit(varDeclCtx->expr()); // résultat dans %eax

            std::string name = varDeclCtx->VAR()->getText();
            int idx = symbolTable[name];
            std::cout << "    movl  %eax, " << idx << "(%rbp)\n";
        }
    }

    return 0;
}

// x = expr;  → evaluate expr into %eax, then store at x's slot
antlrcpp::Any CodeGenVisitor::visitAssign_stmt(ifccParser::Assign_stmtContext *ctx)
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

antlrcpp::Any CodeGenVisitor::visitExpr(ifccParser::ExprContext *ctx)
{
    return visitChildren(ctx);
}

// orExpr: xorExpr ( '|' xorExpr )*
antlrcpp::Any CodeGenVisitor::visitOrExpr(ifccParser::OrExprContext *ctx)
{
    auto operands = ctx->xorExpr();
    visit(operands[0]);

    for (size_t i = 1; i < operands.size(); i++) {
        pushRax();
        visit(operands[i]);
        popRcx();
        std::cout << "    orl   %ecx, %eax\n";
    }
    return 0;
}

// xorExpr: andExpr ( '^' andExpr )*
antlrcpp::Any CodeGenVisitor::visitXorExpr(ifccParser::XorExprContext *ctx)
{
    auto operands = ctx->andExpr();
    visit(operands[0]);

    for (size_t i = 1; i < operands.size(); i++) {
        pushRax();
        visit(operands[i]);
        popRcx();
        std::cout << "    xorl  %ecx, %eax\n";
    }
    return 0;
}

// andExpr: eqExpr ( '&' eqExpr )*
antlrcpp::Any CodeGenVisitor::visitAndExpr(ifccParser::AndExprContext *ctx)
{
    auto operands = ctx->eqExpr();
    visit(operands[0]);

    for (size_t i = 1; i < operands.size(); i++) {
        pushRax();
        visit(operands[i]);
        popRcx();
        std::cout << "    andl  %ecx, %eax\n";
    }
    return 0;
}

// eqExpr: relExpr ( ('==' | '!=') relExpr )*
antlrcpp::Any CodeGenVisitor::visitEqExpr(ifccParser::EqExprContext *ctx)
{
    auto operands = ctx->relExpr();
    visit(operands[0]);

    for (size_t i = 1; i < operands.size(); i++) {
        std::string op = ctx->children[2 * i - 1]->getText();

        pushRax();
        visit(operands[i]);
        popRcx();

        std::cout << "    cmpl  %eax, %ecx\n";
        if (op == "==") {
            std::cout << "    sete  %al\n";
        } else {
            std::cout << "    setne %al\n";
        }
        std::cout << "    movzbl %al, %eax\n";
    }
    return 0;
}

// relExpr: addExpr ( ('<' | '>') addExpr )*
antlrcpp::Any CodeGenVisitor::visitRelExpr(ifccParser::RelExprContext *ctx)
{
    auto operands = ctx->addExpr();
    visit(operands[0]);

    for (size_t i = 1; i < operands.size(); i++) {
        std::string op = ctx->children[2 * i - 1]->getText();

        pushRax();
        visit(operands[i]);
        popRcx();

        std::cout << "    cmpl  %eax, %ecx\n";
        if (op == "<") {
            std::cout << "    setl  %al\n";
        } else {
            std::cout << "    setg  %al\n";
        }
        std::cout << "    movzbl %al, %eax\n";
    }
    return 0;
}

// addExpr: mulExpr ( ('+' | '-') mulExpr )*
antlrcpp::Any CodeGenVisitor::visitAddExpr(ifccParser::AddExprContext *ctx)
{
    auto operands = ctx->mulExpr();
    visit(operands[0]);

    for (size_t i = 1; i < operands.size(); i++) {
        std::string op = ctx->children[2 * i - 1]->getText();

        pushRax();
        visit(operands[i]);
        popRcx();

        if (op == "+") {
            std::cout << "    addl  %ecx, %eax\n";
        } else {
            // left - right = ecx - eax
            std::cout << "    subl  %eax, %ecx\n";
            std::cout << "    movl  %ecx, %eax\n";
        }
    }
    return 0;
}

// mulExpr: unaryExpr ( ('*' | '/' | '%') unaryExpr )*
antlrcpp::Any CodeGenVisitor::visitMulExpr(ifccParser::MulExprContext *ctx)
{
    auto operands = ctx->unaryExpr();
    visit(operands[0]);

    for (size_t i = 1; i < operands.size(); i++) {
        std::string op = ctx->children[2 * i - 1]->getText();

        if (op == "*") {
            pushRax();
            visit(operands[i]);
            popRcx();
            std::cout << "    imull %ecx, %eax\n";
        } 
    }
    return 0;
}

// unaryExpr: ('!' | '-') unaryExpr | primary
antlrcpp::Any CodeGenVisitor::visitUnaryExpr(
    ifccParser::UnaryExprContext *ctx)
{
    if (ctx->primary() != nullptr) {
        return visit(ctx->primary());
    }

    visit(ctx->unaryExpr());

    std::string op = ctx->children[0]->getText();
    if (op == "-") {
        std::cout << "    negl  %eax\n";
    } else if (op == "!") {
        std::cout << "    cmpl  $0, %eax\n";
        std::cout << "    sete  %al\n";
        std::cout << "    movzbl %al, %eax\n";
    }
    return 0;
}

// primary: CONST | VAR | '(' expr ')'
antlrcpp::Any CodeGenVisitor::visitPrimary(ifccParser::PrimaryContext *ctx)
{
    if (ctx->CONST() != nullptr) {
        int val = std::stoi(ctx->CONST()->getText());
        std::cout << "    movl  $" << val << ", %eax\n";
        return 0;
    }

    if (ctx->VAR() != nullptr) {
        std::string name = ctx->VAR()->getText();
        int idx = symbolTable[name];
        std::cout << "    movl  " << idx << "(%rbp), %eax\n";
        return 0;
    }

    return visit(ctx->expr());
}