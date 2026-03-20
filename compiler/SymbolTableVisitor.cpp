#include "SymbolTableVisitor.h"

#include <iostream>
#include <string>

antlrcpp::Any SymbolTableVisitor::visitProg(ifccParser::ProgContext *ctx)
{
    functionTable["getchar"] = FunctionInfo{0, true};
    functionTable["putchar"] = FunctionInfo{1, true};

    bool hasMain = false;

    for (auto fn : ctx->function_def())
    {
        std::string name = fn->VAR()->getText();
        int arity = 0;
        if (fn->param_list() != nullptr)
        {
            arity = static_cast<int>(fn->param_list()->param().size());
        }

        if (arity > 6)
        {
            std::cerr << "error: function '" << name
                      << "' has more than 6 parameters, which is not supported yet\n";
            success = false;
        }

        if (functionTable.count(name))
        {
            std::cerr << "error: function '" << name
                      << "' declared more than once\n";
            success = false;
            continue;
        }

        functionTable[name] = FunctionInfo{arity, false};
        if (name == "main")
        {
            hasMain = true;
        }
    }

    if (!hasMain)
    {
        std::cerr << "error: missing function 'main'\n";
        success = false;
    }

    for (auto fn : ctx->function_def())
    {
        visit(fn);
    }

    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitFunction_def(
    ifccParser::Function_defContext *ctx)
{
    currentFunction = ctx->VAR()->getText();
    currentSymbolTable.clear();
    currentUsed.clear();
    currentNumMaxTemps = 0;
    currentNextIndex = -4;

    if (ctx->param_list() != nullptr)
    {
        for (auto param : ctx->param_list()->param())
        {
            std::string name = param->VAR()->getText();
            if (currentSymbolTable.count(name))
            {
                std::cerr << "error: parameter '" << name
                          << "' declared more than once in function '"
                          << currentFunction << "'\n";
                success = false;
                continue;
            }

            currentSymbolTable[name] = currentNextIndex;
            currentNextIndex -= 4;
        }
    }

    visit(ctx->block());

    for (int i = 0; i < currentNumMaxTemps; i++)
    {
        std::string name = "_temp" + std::to_string(i);
        currentSymbolTable[name] = currentNextIndex;
        currentNextIndex -= 4;
    }

    for (auto &it : currentSymbolTable)
    {
        std::string name = it.first;

        if (name.rfind("_temp", 0) == 0)
        {
            continue;
        }

        if (currentUsed.find(name) == currentUsed.end())
        {
            std::cerr << "warning: variable '" << name
                      << "' declared but never used in function '"
                      << currentFunction << "'\n";
        }
    }

    std::cerr << "=== Symbol Table (" << currentFunction << ") ===\n";
    for (auto &it : currentSymbolTable)
    {
        std::cerr << "  " << it.first << " -> [rbp" << it.second << "]\n";
    }
    std::cerr << "====================\n";

    if (currentFunction == "main")
    {
        symbolTable = currentSymbolTable;
        used = currentUsed;
        numMaxTemps = currentNumMaxTemps;
        nextIndex = currentNextIndex;
    }

    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitWhile_stmt(ifccParser::While_stmtContext *ctx)
{
    visit(ctx->expr());
    visit(ctx->block());
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitIf_stmt(ifccParser::If_stmtContext *ctx)
{
    visit(ctx->expr());
    visit(ctx->block(0));
    if (ctx->block().size() > 1)
    {
        visit(ctx->block(1));
    }
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitExpr_stmt(ifccParser::Expr_stmtContext *ctx)
{
    visit(ctx->expr());
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitFuncCall(ifccParser::FuncCallContext *ctx)
{
    std::string name = ctx->VAR()->getText();
    int argCount = 0;
    if (ctx->arg_list() != nullptr)
    {
        argCount = static_cast<int>(ctx->arg_list()->expr().size());
    }

    if (!functionTable.count(name))
    {
        std::cerr << "error: function '" << name
                  << "' called but not declared\n";
        success = false;
        return 0;
    }

    if (functionTable[name].arity != argCount)
    {
        std::cerr << "error: function '" << name
                  << "' called with " << argCount
                  << " arguments but expects "
                  << functionTable[name].arity << "\n";
        success = false;
        return 0;
    }

    if (argCount > 6)
    {
        std::cerr << "error: functions with more than 6 arguments are not supported yet\n";
        success = false;
        return 0;
    }

    currentNumMaxTemps += argCount;

    if (ctx->arg_list() != nullptr)
    {
        for (auto e : ctx->arg_list()->expr())
        {
            visit(e);
        }
    }

    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitBlock(ifccParser::BlockContext *ctx)
{
    for (auto s : ctx->stmt())
    {
        visit(s);
    }
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitDecl_stmt(ifccParser::Decl_stmtContext *ctx)
{
    for (auto varDecl : ctx->var_decl_list()->var_decl())
    {
        std::string name = varDecl->VAR()->getText();

        if (currentSymbolTable.count(name))
        {
            std::cerr << "error: variable '" << name
                      << "' declared more than once\n";
            success = false;
            continue;
        }

        currentSymbolTable[name] = currentNextIndex;
        currentNextIndex -= 4;

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

    if (!currentSymbolTable.count(name))
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
    currentNumMaxTemps++;
    visit(ctx->expr(0));
    visit(ctx->expr(1));
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitPlusminus(ifccParser::PlusminusContext *ctx)
{
    currentNumMaxTemps++;
    visit(ctx->expr(0));
    visit(ctx->expr(1));
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitLtgt(ifccParser::LtgtContext *ctx)
{
    currentNumMaxTemps++;
    visit(ctx->expr(0));
    visit(ctx->expr(1));
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitEqneq(ifccParser::EqneqContext *ctx)
{
    currentNumMaxTemps++;
    visit(ctx->expr(0));
    visit(ctx->expr(1));
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitBand(ifccParser::BandContext *ctx)
{
    currentNumMaxTemps++;
    visit(ctx->expr(0));
    visit(ctx->expr(1));
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitBxor(ifccParser::BxorContext *ctx)
{
    currentNumMaxTemps++;
    visit(ctx->expr(0));
    visit(ctx->expr(1));
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitBor(ifccParser::BorContext *ctx)
{
    currentNumMaxTemps++;
    visit(ctx->expr(0));
    visit(ctx->expr(1));
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitVarExpr(
    ifccParser::VarExprContext *ctx)
{
    std::string name = ctx->VAR()->getText();

    if (!currentSymbolTable.count(name))
    {
        std::cerr << "error: variable '" << name
                  << "' used before declaration\n";
        success = false;
        return 0;
    }

    currentUsed.insert(name);
    return 0;
}
