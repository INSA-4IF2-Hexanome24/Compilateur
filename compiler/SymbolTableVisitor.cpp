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

antlrcpp::Any SymbolTableVisitor::visitDecl_stmt(ifccParser::Decl_stmtContext *ctx)
{
    // Parcours de toutes les variables déclarées
    auto varListCtx = ctx->var_decl_list();
    for (auto varDeclCtx : varListCtx->var_decl()) {
        std::string name = varDeclCtx->VAR()->getText();

        if (symbolTable.count(name)) {
            std::cerr << "error: variable '" << name
                      << "' declared more than once\n";
            success = false;
            continue;  // passe à la prochaine variable
        }

        symbolTable[name] = nextIndex;
        nextIndex -= 4; // chaque int occupe 4 bytes

        // Si initialisation présente, visite l'expression
        if (varDeclCtx->expr() != nullptr) {
            visit(varDeclCtx->expr());
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

    return visitChildren(ctx);
}