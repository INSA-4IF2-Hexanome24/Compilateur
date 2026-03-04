#pragma once

#include <map>
#include <string>
#include "antlr4-runtime.h"
#include "generated/ifccBaseVisitor.h"

class CodeGenVisitor : public ifccBaseVisitor {
public:
    // Receives the symbol table built by SymbolTableVisitor
    std::map<std::string, int> &symbolTable;
    explicit CodeGenVisitor(std::map<std::string, int> &st) : symbolTable(st) {}

    virtual antlrcpp::Any visitProg(ifccParser::ProgContext *ctx) override;
    virtual antlrcpp::Any visitAssign_stmt(ifccParser::Assign_stmtContext *ctx) override;
    virtual antlrcpp::Any visitReturn_stmt(ifccParser::Return_stmtContext *ctx) override;
    virtual antlrcpp::Any visitConstExpr(ifccParser::ConstExprContext *ctx) override;
    virtual antlrcpp::Any visitVarExpr(ifccParser::VarExprContext *ctx) override;
    virtual antlrcpp::Any visitEqExpr(ifccParser::EqExprContext *ctx) override;
    virtual antlrcpp::Any visitNeExpr(ifccParser::NeExprContext *ctx) override;
    virtual antlrcpp::Any visitLtExpr(ifccParser::LtExprContext *ctx) override;
    virtual antlrcpp::Any visitGtExpr(ifccParser::GtExprContext *ctx) override;
    virtual antlrcpp::Any visitParenExpr(ifccParser::ParenExprContext *ctx) override;
    virtual antlrcpp::Any visitDecl_stmt(ifccParser::Decl_stmtContext *ctx) override;

private:
    int labelCount = 0;
    int getNextLabel() { return labelCount++; }
};