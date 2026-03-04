#pragma once

#include <map>
#include <set>
#include <string>
#include <iostream>
#include "antlr4-runtime.h"
#include "generated/ifccBaseVisitor.h"

class SymbolTableVisitor : public ifccBaseVisitor {
public:
    // Maps variable name -> stack index (negative offset from %rbp, multiple of 4)
    std::map<std::string, int> symbolTable;

    // Returns false if any semantic error was found
    bool success = true;

    virtual antlrcpp::Any visitProg(ifccParser::ProgContext *ctx) override;
    virtual antlrcpp::Any visitDecl_stmt(ifccParser::Decl_stmtContext *ctx) override;
    virtual antlrcpp::Any visitAssign_stmt(ifccParser::Assign_stmtContext *ctx) override;
    virtual antlrcpp::Any visitReturn_stmt(ifccParser::Return_stmtContext *ctx) override;
    virtual antlrcpp::Any visitVarExpr(ifccParser::VarExprContext *ctx) override;
    virtual antlrcpp::Any visitEqExpr(ifccParser::EqExprContext *ctx) override;
    virtual antlrcpp::Any visitNeExpr(ifccParser::NeExprContext *ctx) override;
    virtual antlrcpp::Any visitLtExpr(ifccParser::LtExprContext *ctx) override;
    virtual antlrcpp::Any visitGtExpr(ifccParser::GtExprContext *ctx) override;
    virtual antlrcpp::Any visitParenExpr(ifccParser::ParenExprContext *ctx) override;

private:
    int nextIndex = -4;           // next available stack slot
    std::set<std::string> used;   // variables that have been read
};