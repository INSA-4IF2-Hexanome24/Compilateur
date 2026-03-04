#pragma once

#include <map>
#include <string>

#include "antlr4-runtime.h"
#include "generated/ifccBaseVisitor.h"

class CodeGenVisitor : public ifccBaseVisitor {
public:
    std::map<std::string, int> &symbolTable;

<<<<<<< HEAD
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
=======
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
>>>>>>> 569d447c4d39ce244fa08ddf8fcd96340906064e
};