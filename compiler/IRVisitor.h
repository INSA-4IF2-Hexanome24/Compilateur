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
    std::map<std::string, std::vector<Type>> functionParamTypes;
    std::set<std::string> currentSymbols;
    std::vector<std::set<std::string>> scopeStack;
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
    virtual antlrcpp::Any visitAssignSimple_stmt(
        ifccParser::AssignSimple_stmtContext *ctx) override;
    virtual antlrcpp::Any visitAddAssign_stmt(
        ifccParser::AddAssign_stmtContext *ctx) override;
    virtual antlrcpp::Any visitMinusAssign_stmt(
        ifccParser::MinusAssign_stmtContext *ctx) override;
    virtual antlrcpp::Any visitMultAssign_stmt(
        ifccParser::MultAssign_stmtContext *ctx) override;
    virtual antlrcpp::Any visitDivAssign_stmt(
        ifccParser::DivAssign_stmtContext *ctx) override;
    virtual antlrcpp::Any visitPreMinus_stmt(
        ifccParser::PreMinus_stmtContext *ctx) override;
    virtual antlrcpp::Any visitPreAdd_stmt(
        ifccParser::PreAdd_stmtContext *ctx) override;
    virtual antlrcpp::Any visitPostAdd_stmt(
        ifccParser::PostAdd_stmtContext *ctx) override;
    virtual antlrcpp::Any visitPostMinus_stmt(
        ifccParser::PostMinus_stmtContext *ctx) override;
    virtual antlrcpp::Any visitPtr_assign_stmt(
        ifccParser::Ptr_assign_stmtContext *ctx) override;
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
    virtual antlrcpp::Any visitAddrOfVar(
        ifccParser::AddrOfVarContext *ctx) override;
    virtual antlrcpp::Any visitVarExpr(
        ifccParser::VarExprContext *ctx) override;
    virtual antlrcpp::Any visitDerefExpr(
        ifccParser::DerefExprContext *ctx) override;
    virtual antlrcpp::Any visitFuncCall(
        ifccParser::FuncCallContext *ctx) override;
    virtual antlrcpp::Any visitIf_stmt(ifccParser::If_stmtContext *ctx) override;
    virtual antlrcpp::Any visitBlock(ifccParser::BlockContext *ctx) override;
    virtual antlrcpp::Any visitWhile_stmt(ifccParser::While_stmtContext *ctx) override;

private:
    Type declaredType(const std::string &name) const;
    bool isDeclaredInScope(const std::string &name) const;
};
