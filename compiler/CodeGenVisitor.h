#pragma once

#include <map>
#include <string>
#include "antlr4-runtime.h"
#include "generated/ifccBaseVisitor.h"

class CodeGenVisitor : public ifccBaseVisitor {
public:
    std::map<std::string, int> &symbolTable;

    explicit CodeGenVisitor(std::map<std::string, int> &st)
        : symbolTable(st) {}

    virtual antlrcpp::Any visitProg(ifccParser::ProgContext *ctx) override;
    virtual antlrcpp::Any visitAssign_stmt(ifccParser::Assign_stmtContext *ctx) override;
    virtual antlrcpp::Any visitReturn_stmt(ifccParser::Return_stmtContext *ctx) override;

    virtual antlrcpp::Any visitExpr(ifccParser::ExprContext *ctx) override;
    virtual antlrcpp::Any visitOrExpr(ifccParser::OrExprContext *ctx) override;
    virtual antlrcpp::Any visitXorExpr(ifccParser::XorExprContext *ctx) override;
    virtual antlrcpp::Any visitAndExpr(ifccParser::AndExprContext *ctx) override;
    virtual antlrcpp::Any visitEqExpr(ifccParser::EqExprContext *ctx) override;
    virtual antlrcpp::Any visitRelExpr(ifccParser::RelExprContext *ctx) override;
    virtual antlrcpp::Any visitAddExpr(ifccParser::AddExprContext *ctx) override;
    virtual antlrcpp::Any visitMulExpr(ifccParser::MulExprContext *ctx) override;
    virtual antlrcpp::Any visitUnaryExpr(ifccParser::UnaryExprContext *ctx) override;
    virtual antlrcpp::Any visitPrimary(ifccParser::PrimaryContext *ctx) override;
};
