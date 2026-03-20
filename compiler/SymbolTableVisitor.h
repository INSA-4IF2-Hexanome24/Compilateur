#pragma once

#include "antlr4-runtime.h"
#include "generated/ifccBaseVisitor.h"
#include "generated/ifccParser.h"

#include <map>
#include <set>
#include <string>
#include <vector>

struct FunctionInfo {
    int arity = 0;
    bool isBuiltin = false;
};

class SymbolTableVisitor : public ifccBaseVisitor {
public:
    std::map<std::string, FunctionInfo> functionTable;
    std::map<std::string, int> symbolTable;
    std::set<std::string> used;

    int numMaxTemps = 0;
    int nextIndex = -4;
    bool success = true;

    virtual antlrcpp::Any visitProg(ifccParser::ProgContext *ctx) override;
    virtual antlrcpp::Any visitFunction_def(
        ifccParser::Function_defContext *ctx) override;
    virtual antlrcpp::Any visitExpr_stmt(
        ifccParser::Expr_stmtContext *ctx) override;
    virtual antlrcpp::Any visitDecl_stmt(
        ifccParser::Decl_stmtContext *ctx) override;
    virtual antlrcpp::Any visitAssign_stmt(
        ifccParser::Assign_stmtContext *ctx) override;

    virtual antlrcpp::Any visitMultdivmod(ifccParser::MultdivmodContext *ctx) override;
    virtual antlrcpp::Any visitPlusminus(ifccParser::PlusminusContext *ctx) override;
    virtual antlrcpp::Any visitLtgt(ifccParser::LtgtContext *ctx) override;
    virtual antlrcpp::Any visitEqneq(ifccParser::EqneqContext *ctx) override;
    virtual antlrcpp::Any visitBand(ifccParser::BandContext *ctx) override;
    virtual antlrcpp::Any visitBxor(ifccParser::BxorContext *ctx) override;
    virtual antlrcpp::Any visitBor(ifccParser::BorContext *ctx) override;
    virtual antlrcpp::Any visitVarExpr(
        ifccParser::VarExprContext *ctx) override;
    virtual antlrcpp::Any visitFuncCall(
        ifccParser::FuncCallContext *ctx) override;
    virtual antlrcpp::Any visitIf_stmt(ifccParser::If_stmtContext *ctx) override;
    virtual antlrcpp::Any visitBlock(ifccParser::BlockContext *ctx) override;
    virtual antlrcpp::Any visitWhile_stmt(ifccParser::While_stmtContext *ctx) override;

private:
    std::string currentFunction;
    std::map<std::string, int> currentSymbolTable;
    std::set<std::string> currentUsed;
    int currentNumMaxTemps = 0;
    int currentNextIndex = -4;
};
