#include "SymbolTableVisitor.h"

#include <iostream>
#include <string>

antlrcpp::Any SymbolTableVisitor::visitWhile_stmt(ifccParser::While_stmtContext *ctx)
{
    visit(ctx->expr());

    // visit loop body (always present, index 0)
    visit(ctx->block());

    return 0;
}

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

antlrcpp::Any SymbolTableVisitor::visitExpr_stmt(ifccParser::Expr_stmtContext *ctx)
{
    visit(ctx->expr());
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitFuncCall(ifccParser::FuncCallContext *ctx)
{
    int argCount = 0;
    if (ctx->arg_list() != nullptr) {
        argCount = static_cast<int>(ctx->arg_list()->expr().size());
    }

    if (argCount > 6) {
        std::cerr << "error: functions with more than 6 arguments are not supported yet\n";
        success = false;
        return 0;
    }

    numMaxTemps += argCount;

    if (ctx->arg_list() != nullptr) {
        for (auto e : ctx->arg_list()->expr()) {
            visit(e);
        }
    }

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
    for (auto stmt : ctx->stmt())
    {
        visit(stmt);
    }

    visit(ctx->return_stmt());

    for (int i = 0; i<numMaxTemps;i++)
    {
        std::string name = "_temp"+std::to_string(i);
        symbolTable[name] = nextIndex;
        nextIndex -=4;
    }

    for (auto &it : symbolTable)
    {
        std::string name = it.first;

        if (name.rfind("_temp", 0) == 0)
        {
            continue;
        }

        if (used.find(name) == used.end())
        {
            std::cerr << "warning: variable '" << name
                      << "' declared but never used\n";
        }
    }

    std::cerr << "=== Symbol Table ===\n";
    for (auto &it : symbolTable)
    {
        std::cerr << "  " << it.first << " -> [rbp" << it.second << "]\n";
    }
    std::cerr << "====================\n";

    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitDecl_stmt(ifccParser::Decl_stmtContext *ctx)
{
    for (auto varDecl : ctx->var_decl_list()->var_decl())
    {
        std::string name = varDecl->VAR()->getText();

        if (symbolTable.count(name))
        {
            std::cerr << "error: variable '" << name
                      << "' declared more than once\n";
            success = false;
            continue;
        }

        symbolTable[name] = nextIndex;
        nextIndex -= 4;

        if (varDecl->expr() != nullptr)
        {
            visit(varDecl->expr());
        }
    }

    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitAssign_stmt(
    ifccParser::Assign_stmtContext *ctx)
{
    std::string name = ctx->VAR()->getText();

    if (!symbolTable.count(name))
    {
        std::cerr << "error: variable '" << name
                  << "' used before declaration\n";
        success = false;
        return 0;
    }

    visit(ctx->expr());
    return 0;
}


antlrcpp::Any SymbolTableVisitor::visitMultdivmod(ifccParser::MultdivmodContext *ctx)
{
    numMaxTemps++;
    visit(ctx->expr(0));
    visit(ctx->expr(1));
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitPlusminus(ifccParser::PlusminusContext *ctx)
{
    numMaxTemps++;
    visit(ctx->expr(0));
    visit(ctx->expr(1));
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitLtgt(ifccParser::LtgtContext *ctx)
{
    numMaxTemps++;
    visit(ctx->expr(0));
    visit(ctx->expr(1));
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitEqneq(ifccParser::EqneqContext *ctx)
{
    numMaxTemps++;
    visit(ctx->expr(0));
    visit(ctx->expr(1));
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitBand(ifccParser::BandContext *ctx)
{
    numMaxTemps++;
    visit(ctx->expr(0));
    visit(ctx->expr(1));
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitBxor(ifccParser::BxorContext *ctx)
{
    numMaxTemps++;
    visit(ctx->expr(0));
    visit(ctx->expr(1));
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitBor(ifccParser::BorContext *ctx)
{
    numMaxTemps++;
    visit(ctx->expr(0));
    visit(ctx->expr(1));
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitVarExpr(
    ifccParser::VarExprContext *ctx)
{
    std::string name = ctx->VAR()->getText();

    if (!symbolTable.count(name))
    {
        std::cerr << "error: variable '" << name
                  << "' used before declaration\n";
        success = false;
        return 0;
    }

    used.insert(name);
    return 0;
}
