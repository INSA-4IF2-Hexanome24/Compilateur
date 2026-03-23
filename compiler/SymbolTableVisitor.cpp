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
    std::pair<int,int>* parent = nullptr;  // pointeur, null si pas de parent
    std::pair<int,int> enfant;

    if (!scopeStack.empty())
    {
        parent = &scopeStack.back();
        enfant = {parent->second, 0};
    }
    else
    {
        enfant = {0, 0};
    }

    scopeStack.push_back(enfant);

    for (auto s : ctx->stmt())
        visit(s);

    scopeStack.pop_back();

    if (parent != nullptr)
        parent->second++;

    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitProg(ifccParser::ProgContext *ctx)
{
    for (auto stmt : ctx->stmt())
    {
        visit(stmt);
    }

    //visit(ctx->return_stmt());

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
        name = currentPrefix() + name; 

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

    name = resolveVar(name);

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
    name = resolveVar(name);

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



    std::string SymbolTableVisitor::currentPrefix()
    {
        std::vector<std::pair<int,int>>::iterator it;  
        std::string resultat ="";
        for(it = scopeStack.begin(); it != scopeStack.end(); it++ )
        {
            resultat += std::to_string(it->first) + "-";
        }
        return resultat;
    }

std::string SymbolTableVisitor::resolveVar(std::string var)
{
    // essaie du scope le plus profond au plus global
    for (int i = scopeStack.size(); i >= 0; i--)
    {
        // construire le préfixe avec les i premiers éléments de scopeStack
        std::string prefix = "";
        for (int j = 0; j < i; j++)
        {
            prefix += std::to_string(scopeStack[j].first) + "-";
        }
        std::string candidate = prefix + var;
        if (symbolTable.count(candidate))
            return candidate;
    }
    return var; // pas trouvé
}