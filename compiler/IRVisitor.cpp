#include "CodeGenVisitor.h"
#include "IR.h"

#include <iostream>
#include <string>
#include <vector>

using std::string;
using std::vector;

CodeGenVisitor::CodeGenVisitor()
{
    numMaxTemps = 0;
}

CodeGenVisitor::CodeGenVisitor(const std::map<std::string, int> &table, int numMaxTemps)
{
    symbolTable = table;
    this->numMaxTemps = numMaxTemps;
}

antlrcpp::Any CodeGenVisitor::visitProg(ifccParser::ProgContext *ctx)
{
    cfg = new CFG(nullptr);

    for (const auto &it : symbolTable)
    {
        const string &name = it.first;
        if (name.rfind("_temp", 0) == 0)
        {
            continue;
        }
        cfg->add_to_symbol_table(name, TYPE_INT);
    }

    BasicBlock *entry = new BasicBlock(cfg, "entry");
    cfg->add_bb(entry);
    cfg->current_bb = entry;

    returnBB = new BasicBlock(cfg, "ret_block");
    cfg->add_bb(returnBB);

    for (auto s : ctx->stmt())
    {
        visit(s);
    }

    if (cfg->current_bb->exit_true == nullptr && cfg->current_bb->exit_false == nullptr)
    {
        string zero = cfg->create_new_tempvar(TYPE_INT);
        cfg->current_bb->add_IRInstr(IRInstr::ldconst, TYPE_INT, {zero, "0"});
        cfg->current_bb->add_IRInstr(IRInstr::ret, TYPE_INT, {zero});
        cfg->current_bb->exit_true = returnBB;
    }

    cfg->gen_asm(std::cout);
    return 0;
}

antlrcpp::Any CodeGenVisitor::visitExpr_stmt(ifccParser::Expr_stmtContext *ctx)
{
    visit(ctx->expr());
    return 0;
}

antlrcpp::Any CodeGenVisitor::visitDecl_stmt(ifccParser::Decl_stmtContext *ctx)
{
    for (auto v : ctx->var_decl_list()->var_decl())
    {
        if (v->expr() != nullptr)
        {
            string rhs = visit(v->expr()).as<string>();
            string lhs = v->VAR()->getText();
            cfg->current_bb->add_IRInstr(IRInstr::copy, TYPE_INT, {rhs, lhs});
        }
    }
    return 0;
}

antlrcpp::Any CodeGenVisitor::visitAssign_stmt(ifccParser::Assign_stmtContext *ctx)
{
    string rhs = visit(ctx->expr()).as<string>();
    string lhs = ctx->VAR()->getText();
    cfg->current_bb->add_IRInstr(IRInstr::copy, TYPE_INT, {rhs, lhs});
    return 0;
}

antlrcpp::Any CodeGenVisitor::visitReturn_stmt(ifccParser::Return_stmtContext *ctx)
{
    string retVal = visit(ctx->expr()).as<string>();
    cfg->current_bb->add_IRInstr(IRInstr::ret, TYPE_INT, {retVal});
    cfg->current_bb->exit_true = returnBB;

    BasicBlock *dead = new BasicBlock(cfg, cfg->new_BB_name());
    cfg->add_bb(dead);
    cfg->current_bb = dead;
    return 0;
}

antlrcpp::Any CodeGenVisitor::visitMultdivmod(ifccParser::MultdivmodContext *ctx)
{
    string lhs = visit(ctx->expr(0)).as<string>();
    string rhs = visit(ctx->expr(1)).as<string>();
    string dst = cfg->create_new_tempvar(TYPE_INT);
    string op = ctx->OP->getText();

    if (op == "*")
    {
        cfg->current_bb->add_IRInstr(IRInstr::mul, TYPE_INT, {lhs, rhs, dst});
    }
    else if (op == "/")
    {
        cfg->current_bb->add_IRInstr(IRInstr::div, TYPE_INT, {lhs, rhs, dst});
    }
    else
    {
        cfg->current_bb->add_IRInstr(IRInstr::mod, TYPE_INT, {lhs, rhs, dst});
    }
    return dst;
}

antlrcpp::Any CodeGenVisitor::visitPlusminus(ifccParser::PlusminusContext *ctx)
{
    string lhs = visit(ctx->expr(0)).as<string>();
    string rhs = visit(ctx->expr(1)).as<string>();
    string dst = cfg->create_new_tempvar(TYPE_INT);
    string op = ctx->OP->getText();

    if (op == "+")
    {
        cfg->current_bb->add_IRInstr(IRInstr::add, TYPE_INT, {lhs, rhs, dst});
    }
    else
    {
        cfg->current_bb->add_IRInstr(IRInstr::sub, TYPE_INT, {lhs, rhs, dst});
    }
    return dst;
}

antlrcpp::Any CodeGenVisitor::visitLtgt(ifccParser::LtgtContext *ctx)
{
    string lhs = visit(ctx->expr(0)).as<string>();
    string rhs = visit(ctx->expr(1)).as<string>();
    string dst = cfg->create_new_tempvar(TYPE_INT);
    string op = ctx->OP->getText();

    if (op == "<")
    {
        cfg->current_bb->add_IRInstr(IRInstr::cmp_lt, TYPE_INT, {lhs, rhs, dst});
    }
    else
    {
        cfg->current_bb->add_IRInstr(IRInstr::cmp_lt, TYPE_INT, {rhs, lhs, dst});
    }
    return dst;
}

antlrcpp::Any CodeGenVisitor::visitEqneq(ifccParser::EqneqContext *ctx)
{
    string lhs = visit(ctx->expr(0)).as<string>();
    string rhs = visit(ctx->expr(1)).as<string>();
    string dst = cfg->create_new_tempvar(TYPE_INT);
    string op = ctx->OP->getText();

    if (op == "==")
    {
        cfg->current_bb->add_IRInstr(IRInstr::cmp_eq, TYPE_INT, {lhs, rhs, dst});
    }
    else
    {
        cfg->current_bb->add_IRInstr(IRInstr::cmp_ne, TYPE_INT, {lhs, rhs, dst});
    }
    return dst;
}

antlrcpp::Any CodeGenVisitor::visitBand(ifccParser::BandContext *ctx)
{
    string lhs = visit(ctx->expr(0)).as<string>();
    string rhs = visit(ctx->expr(1)).as<string>();
    string dst = cfg->create_new_tempvar(TYPE_INT);
    cfg->current_bb->add_IRInstr(IRInstr::band, TYPE_INT, {lhs, rhs, dst});
    return dst;
}

antlrcpp::Any CodeGenVisitor::visitBxor(ifccParser::BxorContext *ctx)
{
    string lhs = visit(ctx->expr(0)).as<string>();
    string rhs = visit(ctx->expr(1)).as<string>();
    string dst = cfg->create_new_tempvar(TYPE_INT);
    cfg->current_bb->add_IRInstr(IRInstr::bxor, TYPE_INT, {lhs, rhs, dst});
    return dst;
}

antlrcpp::Any CodeGenVisitor::visitBor(ifccParser::BorContext *ctx)
{
    string lhs = visit(ctx->expr(0)).as<string>();
    string rhs = visit(ctx->expr(1)).as<string>();
    string dst = cfg->create_new_tempvar(TYPE_INT);
    cfg->current_bb->add_IRInstr(IRInstr::bor, TYPE_INT, {lhs, rhs, dst});
    return dst;
}

antlrcpp::Any CodeGenVisitor::visitNotExpr(ifccParser::NotExprContext *ctx)
{
    string val = visit(ctx->expr()).as<string>();
    string zero = cfg->create_new_tempvar(TYPE_INT);
    string dst = cfg->create_new_tempvar(TYPE_INT);
    cfg->current_bb->add_IRInstr(IRInstr::ldconst, TYPE_INT, {zero, "0"});
    cfg->current_bb->add_IRInstr(IRInstr::cmp_eq, TYPE_INT, {val, zero, dst});
    return dst;
}

antlrcpp::Any CodeGenVisitor::visitUnaryMinus(ifccParser::UnaryMinusContext *ctx)
{
    string val = visit(ctx->expr()).as<string>();
    string zero = cfg->create_new_tempvar(TYPE_INT);
    string dst = cfg->create_new_tempvar(TYPE_INT);
    cfg->current_bb->add_IRInstr(IRInstr::ldconst, TYPE_INT, {zero, "0"});
    cfg->current_bb->add_IRInstr(IRInstr::sub, TYPE_INT, {zero, val, dst});
    return dst;
}

antlrcpp::Any CodeGenVisitor::visitParens(ifccParser::ParensContext *ctx)
{
    return visit(ctx->expr());
}

antlrcpp::Any CodeGenVisitor::visitConstExpr(ifccParser::ConstExprContext *ctx)
{
    string dst = cfg->create_new_tempvar(TYPE_INT);
    cfg->current_bb->add_IRInstr(IRInstr::ldconst, TYPE_INT, {dst, ctx->CONST()->getText()});
    return dst;
}

antlrcpp::Any CodeGenVisitor::visitVarExpr(ifccParser::VarExprContext *ctx)
{
    return ctx->VAR()->getText();
}

antlrcpp::Any CodeGenVisitor::visitFuncCall(ifccParser::FuncCallContext *ctx)
{
    vector<string> params;
    params.push_back(ctx->VAR()->getText());

    string dst = cfg->create_new_tempvar(TYPE_INT);
    params.push_back(dst);

    if (ctx->arg_list() != nullptr)
    {
        for (auto e : ctx->arg_list()->expr())
        {
            params.push_back(visit(e).as<string>());
        }
    }

    cfg->current_bb->add_IRInstr(IRInstr::call, TYPE_INT, params);
    return dst;
}

antlrcpp::Any CodeGenVisitor::visitIf_stmt(ifccParser::If_stmtContext *ctx)
{
    string cond = visit(ctx->expr()).as<string>();

    BasicBlock *thenBB = new BasicBlock(cfg, cfg->new_BB_name());
    BasicBlock *afterBB = new BasicBlock(cfg, cfg->new_BB_name());
    cfg->add_bb(thenBB);

    cfg->current_bb->test_var_name = cond;

    if (ctx->block().size() > 1)
    {
        BasicBlock *elseBB = new BasicBlock(cfg, cfg->new_BB_name());
        cfg->add_bb(elseBB);
        cfg->add_bb(afterBB);

        cfg->current_bb->exit_true = thenBB;
        cfg->current_bb->exit_false = elseBB;

        cfg->current_bb = thenBB;
        visit(ctx->block(0));
        if (thenBB->exit_true == nullptr && thenBB->exit_false == nullptr)
        {
            thenBB->exit_true = afterBB;
        }

        cfg->current_bb = elseBB;
        visit(ctx->block(1));
        if (elseBB->exit_true == nullptr && elseBB->exit_false == nullptr)
        {
            elseBB->exit_true = afterBB;
        }
    }
    else
    {
        cfg->add_bb(afterBB);
        cfg->current_bb->exit_true = thenBB;
        cfg->current_bb->exit_false = afterBB;

        cfg->current_bb = thenBB;
        visit(ctx->block(0));
        if (thenBB->exit_true == nullptr && thenBB->exit_false == nullptr)
        {
            thenBB->exit_true = afterBB;
        }
    }

    cfg->current_bb = afterBB;
    return 0;
}

antlrcpp::Any CodeGenVisitor::visitBlock(ifccParser::BlockContext *ctx)
{
    for (auto s : ctx->stmt())
    {
        visit(s);
    }
    return 0;
}

antlrcpp::Any CodeGenVisitor::visitWhile_stmt(ifccParser::While_stmtContext *ctx)
{
    BasicBlock *condBB = new BasicBlock(cfg, cfg->new_BB_name());
    BasicBlock *bodyBB = new BasicBlock(cfg, cfg->new_BB_name());
    BasicBlock *afterBB = new BasicBlock(cfg, cfg->new_BB_name());

    cfg->add_bb(condBB);
    cfg->add_bb(bodyBB);
    cfg->add_bb(afterBB);

    cfg->current_bb->exit_true = condBB;

    cfg->current_bb = condBB;
    string cond = visit(ctx->expr()).as<string>();
    condBB->test_var_name = cond;
    condBB->exit_true = bodyBB;
    condBB->exit_false = afterBB;

    cfg->current_bb = bodyBB;
    visit(ctx->block());
    if (bodyBB->exit_true == nullptr && bodyBB->exit_false == nullptr)
    {
        bodyBB->exit_true = condBB;
    }

    cfg->current_bb = afterBB;
    return 0;
}
