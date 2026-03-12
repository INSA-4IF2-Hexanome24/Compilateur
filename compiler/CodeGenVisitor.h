#pragma once

#include "antlr4-runtime.h"
#include "generated/ifccBaseVisitor.h"
#include "generated/ifccParser.h"

#include <map>
#include <string>

class CodeGenVisitor : public ifccBaseVisitor {
public:
    std::map<std::string, int> symbolTable;
    int numTemps;
    int labelCount = 0;

    CodeGenVisitor();
    CodeGenVisitor(const std::map<std::string, int> &table);

    void setTempVar(int i);
    void getTempVar(int i);

    virtual antlrcpp::Any visitProg(ifccParser::ProgContext *ctx) override;
    virtual antlrcpp::Any visitDecl_stmt(
        ifccParser::Decl_stmtContext *ctx) override;
    virtual antlrcpp::Any visitAssign_stmt(
        ifccParser::Assign_stmtContext *ctx) override;
    virtual antlrcpp::Any visitReturn_stmt(
        ifccParser::Return_stmtContext *ctx) override;

    virtual antlrcpp::Any visitMultdivmod(ifccParser::MultdivmodContext *ctx) override;
    virtual antlrcpp::Any visitPlusminus(ifccParser::PlusminusContext *ctx) override;
    virtual antlrcpp::Any visitLtgt(ifccParser::LtgtContext *ctx) override;
    virtual antlrcpp::Any visitEqneq(ifccParser::EqneqContext *ctx) override;
    virtual antlrcpp::Any visitBand(ifccParser::BandContext *ctx) override;
    virtual antlrcpp::Any visitBxor(ifccParser::BxorContext *ctx) override;
    virtual antlrcpp::Any visitBor(ifccParser::BorContext *ctx) override;
    virtual antlrcpp::Any visitNotExpr(
        ifccParser::NotExprContext *ctx) override;
    virtual antlrcpp::Any visitUnaryMinus(
        ifccParser::UnaryMinusContext *ctx) override;
    virtual antlrcpp::Any visitParens(ifccParser::ParensContext *ctx) override;
    virtual antlrcpp::Any visitConstExpr(
        ifccParser::ConstExprContext *ctx) override;
    virtual antlrcpp::Any visitVarExpr(
        ifccParser::VarExprContext *ctx) override;
    virtual antlrcpp::Any visitIf_stmt(ifccParser::If_stmtContext *ctx) override;
    virtual antlrcpp::Any visitBlock(ifccParser::BlockContext *ctx) override;
    virtual antlrcpp::Any visitWhile_stmt(ifccParser::While_stmtContext *ctx) override;
};