#include "SymbolTableVisitor.h"

#include <iostream>
#include <string>


antlrcpp::Any SymbolTableVisitor::visitIf_stmt(ifccParser::If_stmtContext *ctx)
{
    visit(ctx->expr());

    // visit then-block (always present, index 0)
    visit(ctx->block(0));

    // visit else-block if present
    if (ctx->block().size() > 1)
        visit(ctx->block(1));

    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitBlock(ifccParser::BlockContext *ctx)
{
    for (auto s : ctx->stmt())
        visit(s);
    return 0;
}


antlrcpp::Any SymbolTableVisitor::visitProg(ifccParser::ProgContext *ctx)
{
    for (int i = 0; i < 20; i++) {
    std::string name = "_temp" + std::to_string(i);
    symbolTable[name] = nextIndex;
    nextIndex -= 4;
    }

    for (auto stmt : ctx->stmt()) {
        visit(stmt);
    }

    visit(ctx->return_stmt());

    for (auto &it : symbolTable) {
        std::string name = it.first;

        if (used.find(name) == used.end()) {
            std::cerr << "warning: variable '" << name
                      << "' declared but never used\n";
        }
    }

    std::cerr << "=== Symbol Table ===\n";
    for (auto &it : symbolTable) {
        std::cerr << "  " << it.first << " -> [rbp" << it.second << "]\n";
    }
    std::cerr << "====================\n";

    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitDecl_stmt(ifccParser::Decl_stmtContext *ctx)
{
    for (auto varDecl : ctx->var_decl_list()->var_decl()) {
        std::string name = varDecl->VAR()->getText();

        if (symbolTable.count(name)) {
            std::cerr << "error: variable '" << name
                      << "' declared more than once\n";
            success = false;
            continue;
        }

        symbolTable[name] = nextIndex;
        nextIndex -= 4;

        if (varDecl->expr() != nullptr) {
            visit(varDecl->expr());
        }
    }

    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitAssign_stmt(
    ifccParser::Assign_stmtContext *ctx)
{
    std::string name = ctx->VAR()->getText();

    if (!symbolTable.count(name)) {
        std::cerr << "error: variable '" << name
                  << "' used before declaration\n";
        success = false;
        return 0;
    }

    visit(ctx->expr());
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitReturn_stmt(
    ifccParser::Return_stmtContext *ctx)
{
    visit(ctx->expr());
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitMultdiv(ifccParser::MultdivContext *ctx)
{
    visit(ctx->expr(0));
    visit(ctx->expr(1));
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitDiv(ifccParser::DivContext *ctx)
{
    visit(ctx->expr(0));
    visit(ctx->expr(1));
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitMod(ifccParser::ModContext *ctx)
{
    visit(ctx->expr(0));
    visit(ctx->expr(1));
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitPlus(ifccParser::PlusContext *ctx)
{
    visit(ctx->expr(0));
    visit(ctx->expr(1));
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitMinus(ifccParser::MinusContext *ctx)
{
    visit(ctx->expr(0));
    visit(ctx->expr(1));
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitLt(ifccParser::LtContext *ctx)
{
    visit(ctx->expr(0));
    visit(ctx->expr(1));
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitGt(ifccParser::GtContext *ctx)
{
    visit(ctx->expr(0));
    visit(ctx->expr(1));
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitEq(ifccParser::EqContext *ctx)
{
    visit(ctx->expr(0));
    visit(ctx->expr(1));
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitNeq(ifccParser::NeqContext *ctx)
{
    visit(ctx->expr(0));
    visit(ctx->expr(1));
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitBand(ifccParser::BandContext *ctx)
{
    visit(ctx->expr(0));
    visit(ctx->expr(1));
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitBxor(ifccParser::BxorContext *ctx)
{
    visit(ctx->expr(0));
    visit(ctx->expr(1));
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitBor(ifccParser::BorContext *ctx)
{
    visit(ctx->expr(0));
    visit(ctx->expr(1));
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitNotExpr(
    ifccParser::NotExprContext *ctx)
{
    visit(ctx->expr());
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitUnaryMinus(
    ifccParser::UnaryMinusContext *ctx)
{
    visit(ctx->expr());
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitParens(ifccParser::ParensContext *ctx)
{
    visit(ctx->expr());
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitConstExpr(
    ifccParser::ConstExprContext *ctx)
{
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitVarExpr(
    ifccParser::VarExprContext *ctx)
{
    std::string name = ctx->VAR()->getText();

    if (!symbolTable.count(name)) {
        std::cerr << "error: variable '" << name
                  << "' used before declaration\n";
        success = false;
        return 0;
    }

    used.insert(name);
    return 0;
}