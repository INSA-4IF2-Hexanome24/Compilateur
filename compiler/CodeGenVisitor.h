#pragma once

#include <map>
#include <string>

#include "antlr4-runtime.h"
#include "generated/ifccBaseVisitor.h"

class CodeGenVisitor : public ifccBaseVisitor {
public:
    std::map<std::string, int> &symbolTable;

    int numTemps =0;
    int maxTempTop=0;

    explicit CodeGenVisitor(std::map<std::string, int> &st)
        : symbolTable(st)
    {
    }

    antlrcpp::Any visitProg(ifccParser::ProgContext *ctx) override;

    antlrcpp::Any visitDecl_stmt(ifccParser::Decl_stmtContext *ctx) override;
    antlrcpp::Any visitAssign_stmt(ifccParser::Assign_stmtContext *ctx) override;
    antlrcpp::Any visitReturn_stmt(ifccParser::Return_stmtContext *ctx) override;

    antlrcpp::Any visitExpr(ifccParser::ExprContext *ctx) override;
    antlrcpp::Any visitOrExpr(ifccParser::OrExprContext *ctx) override;
    antlrcpp::Any visitXorExpr(ifccParser::XorExprContext *ctx) override;
    antlrcpp::Any visitAndExpr(ifccParser::AndExprContext *ctx) override;
    antlrcpp::Any visitEqExpr(ifccParser::EqExprContext *ctx) override;
    antlrcpp::Any visitRelExpr(ifccParser::RelExprContext *ctx) override;
    antlrcpp::Any visitAddExpr(ifccParser::AddExprContext *ctx) override;
    antlrcpp::Any visitMulExpr(ifccParser::MulExprContext *ctx) override;
    antlrcpp::Any visitUnaryExpr(ifccParser::UnaryExprContext *ctx) override;
    antlrcpp::Any visitPrimary(ifccParser::PrimaryContext *ctx) override;


    void getTempVar(int i);
    void setTempVar(int i);
};