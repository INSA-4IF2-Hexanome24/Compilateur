#include "SymbolTableVisitor.h"

antlrcpp::Any SymbolTableVisitor::visitProg(ifccParser::ProgContext *ctx)
{
    // Visit all children (stmts + return)
    visitChildren(ctx);

    // Check: every declared variable must be used at least once
    for (auto &[name, idx] : symbolTable) {
        if (used.find(name) == used.end()) {
            std::cerr << "warning: variable '" << name << "' declared but never used\n";
            // Treated as warning only; set success=false if you want it fatal
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

antlrcpp::Any SymbolTableVisitor::visitDecl_stmt(ifccParser::Decl_stmtContext *ctx)
{
    std::string name = ctx->VAR()->getText();
    ifccParser::ExprContext *expr = ctx->expr();

    if (expr) {
        visit(expr);
    }

    if (symbolTable.count(name)) {
        std::cerr << "error: variable '" << name << "' declared more than once\n";
        success = false;
        return 0;
    }

    symbolTable[name] = nextIndex;
    nextIndex -= 4;   // each int occupies 4 bytes
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitAssign_stmt(ifccParser::Assign_stmtContext *ctx)
{
    std::string name = ctx->VAR()->getText();

    if (!symbolTable.count(name)) {
        std::cerr << "error: variable '" << name << "' used before declaration\n";
        success = false;
        return 0;
    }

    // Visit the right-hand side expression (may contain a VAR)
    visit(ctx->expr());
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitReturn_stmt(ifccParser::Return_stmtContext *ctx)
{
    visit(ctx->expr());
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitVarExpr(ifccParser::VarExprContext *ctx)
{
    std::string name = ctx->VAR()->getText();

    if (!symbolTable.count(name)) {
        std::cerr << "error: variable '" << name << "' used before declaration\n";
        success = false;
        return 0;
    }

    used.insert(name);
    return 0;
}