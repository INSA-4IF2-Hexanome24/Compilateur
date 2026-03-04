#include "SymbolTableVisitor.h"

antlrcpp::Any SymbolTableVisitor::visitProg(ifccParser::ProgContext *ctx)
{
    // Visit all children (stmts + return)
    visitChildren(ctx);

    // Check: every declared variable must be used at least once (read at least once)
    for (auto &[name, idx] : symbolTable) {
        (void)idx;
        if (used.find(name) == used.end()) {
            std::cerr << "warning: variable '" << name
                      << "' declared but never used\n";
        }
    }

    // Debug: print the full symbol table
    std::cerr << "=== Symbol Table ===\n";
    for (auto &[name, idx] : symbolTable) {
        std::cerr << "  " << name << "  ->  [rbp" << idx << "]\n";
    }
    std::cerr << "====================\n";

    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitDecl_stmt(
    ifccParser::Decl_stmtContext *ctx)
{
    std::string name = ctx->VAR()->getText();

    if (symbolTable.count(name)) {
        std::cerr << "error: variable '" << name
                  << "' declared more than once\n";
        success = false;
        return 0;
    }

    symbolTable[name] = nextIndex;
<<<<<<< HEAD
    nextIndex -= 4;   // each int occupies 4 bytes

    if (ctx->expr()) {
        visit(ctx->expr());
    }
=======
    nextIndex -= 4; // each int occupies 4 bytes

    // If there is an initializer, visit it (to validate vars used in expr).
    if (ctx->expr() != nullptr) {
        visit(ctx->expr());
    }

>>>>>>> 569d447c4d39ce244fa08ddf8fcd96340906064e
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

    // Visit the right-hand side expression (may contain VAR reads)
    visit(ctx->expr());
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitReturn_stmt(
    ifccParser::Return_stmtContext *ctx)
{
    visit(ctx->expr());
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitPrimary(
    ifccParser::PrimaryContext *ctx)
{
    if (ctx->VAR() != nullptr) {
        std::string name = ctx->VAR()->getText();

        if (!symbolTable.count(name)) {
            std::cerr << "error: variable '" << name
                      << "' used before declaration\n";
            success = false;
            return 0;
        }

        used.insert(name);
    }

<<<<<<< HEAD
    used.insert(name);
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitEqExpr(ifccParser::EqExprContext *ctx)
{
    visit(ctx->expr(0));
    visit(ctx->expr(1));
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitNeExpr(ifccParser::NeExprContext *ctx)
{
    visit(ctx->expr(0));
    visit(ctx->expr(1));
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitLtExpr(ifccParser::LtExprContext *ctx)
{
    visit(ctx->expr(0));
    visit(ctx->expr(1));
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitGtExpr(ifccParser::GtExprContext *ctx)
{
    visit(ctx->expr(0));
    visit(ctx->expr(1));
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitParenExpr(ifccParser::ParenExprContext *ctx)
{
    visit(ctx->expr());
    return 0;
=======
    return visitChildren(ctx);
>>>>>>> 569d447c4d39ce244fa08ddf8fcd96340906064e
}