#pragma once

#include "antlr4-runtime.h"
#include "generated/ifccBaseVisitor.h"
#include "generated/ifccParser.h"
#include "IR.h"

#include <map>
#include <set>
#include <string>
#include <vector>

class IRVisitor : public ifccBaseVisitor {
public:
    std::map<std::string, int> functionArity;
    std::set<std::string> currentSymbols;
    std::string currentFunction;
    CFG *cfg = nullptr;
    BasicBlock *returnBB = nullptr;
    bool success = true;

    IRVisitor();

    virtual antlrcpp::Any visitProg(ifccParser::ProgContext *ctx) override;
    virtual antlrcpp::Any visitFunction_def(
        ifccParser::Function_defContext *ctx) override;
    virtual antlrcpp::Any visitExpr_stmt(
        ifccParser::Expr_stmtContext *ctx) override;
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
    virtual antlrcpp::Any visitFuncCall(
        ifccParser::FuncCallContext *ctx) override;
    virtual antlrcpp::Any visitIf_stmt(ifccParser::If_stmtContext *ctx) override;
    virtual antlrcpp::Any visitBlock(ifccParser::BlockContext *ctx) override;
    virtual antlrcpp::Any visitWhile_stmt(ifccParser::While_stmtContext *ctx) override;
};
