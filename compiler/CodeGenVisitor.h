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

    virtual antlrcpp::Any visitMultdiv(ifccParser::MultdivContext *ctx) override;
    virtual antlrcpp::Any visitDiv(ifccParser::DivContext *ctx) override;
    virtual antlrcpp::Any visitMod(ifccParser::ModContext *ctx) override;
    virtual antlrcpp::Any visitPlus(ifccParser::PlusContext *ctx) override;
    virtual antlrcpp::Any visitMinus(ifccParser::MinusContext *ctx) override;
    virtual antlrcpp::Any visitLt(ifccParser::LtContext *ctx) override;
    virtual antlrcpp::Any visitGt(ifccParser::GtContext *ctx) override;
    virtual antlrcpp::Any visitEq(ifccParser::EqContext *ctx) override;
    virtual antlrcpp::Any visitNeq(ifccParser::NeqContext *ctx) override;
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
};