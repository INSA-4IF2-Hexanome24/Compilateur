#include "IRVisitor.h"
#include "IR.h"

#include <iostream>
#include <string>
#include <vector>

using std::string;
using std::vector;

using namespace std;

IRVisitor::IRVisitor()
{
    numMaxTemps = 0;
}

IRVisitor::IRVisitor(const std::map<std::string, int> &table, int numMaxTemps)
{
    symbolTable = table;
    this->numMaxTemps = numMaxTemps;
}

antlrcpp::Any IRVisitor::visitProg(ifccParser::ProgContext *ctx)
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

antlrcpp::Any IRVisitor::visitExpr_stmt(ifccParser::Expr_stmtContext *ctx)
{
    visit(ctx->expr());
    return 0;
}

antlrcpp::Any IRVisitor::visitDecl_stmt(ifccParser::Decl_stmtContext *ctx)
{
    for (auto v : ctx->var_decl_list()->var_decl())
    {
        if (v->expr() != nullptr)
        {
            string rhs = any_cast<string>(visit(v->expr()));
            string lhs = v->VAR()->getText();
            cfg->current_bb->add_IRInstr(IRInstr::copy, TYPE_INT, {rhs, lhs});
        }
    }
    return 0;
}

antlrcpp::Any IRVisitor::visitAssign_stmt(ifccParser::Assign_stmtContext *ctx)
{
    string rhs = any_cast<string>(visit(ctx->expr()));
    string lhs = ctx->VAR()->getText();
    cfg->current_bb->add_IRInstr(IRInstr::copy, TYPE_INT, {rhs, lhs});
    return 0;
}

antlrcpp::Any IRVisitor::visitReturn_stmt(ifccParser::Return_stmtContext *ctx)
{
    string retVal = any_cast<string>(visit(ctx->expr()));
    cfg->current_bb->add_IRInstr(IRInstr::ret, TYPE_INT, {retVal});
    cfg->current_bb->exit_true = returnBB;

    BasicBlock *dead = new BasicBlock(cfg, cfg->new_BB_name());
    cfg->add_bb(dead);
    cfg->current_bb = dead;
    return 0;
}

antlrcpp::Any IRVisitor::visitMultdivmod(ifccParser::MultdivmodContext *ctx)
{
    string lhs = any_cast<string>(visit(ctx->expr(0)));
    string rhs = any_cast<string>(visit(ctx->expr(1)));
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

antlrcpp::Any IRVisitor::visitPlusminus(ifccParser::PlusminusContext *ctx)
{
    string lhs = any_cast<string>(visit(ctx->expr(0)));
    string rhs = any_cast<string>(visit(ctx->expr(1)));
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

antlrcpp::Any IRVisitor::visitLtgt(ifccParser::LtgtContext *ctx)
{
    string lhs = any_cast<string>(visit(ctx->expr(0)));
    string rhs = any_cast<string>(visit(ctx->expr(1)));
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

antlrcpp::Any IRVisitor::visitEqneq(ifccParser::EqneqContext *ctx)
{
    string lhs = any_cast<string>(visit(ctx->expr(0)));
    string rhs = any_cast<string>(visit(ctx->expr(1)));
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

antlrcpp::Any IRVisitor::visitBand(ifccParser::BandContext *ctx)
{
    string lhs = any_cast<string>(visit(ctx->expr(0)));
    string rhs = any_cast<string>(visit(ctx->expr(1)));
    string dst = cfg->create_new_tempvar(TYPE_INT);
    cfg->current_bb->add_IRInstr(IRInstr::band, TYPE_INT, {lhs, rhs, dst});
    return dst;
}

antlrcpp::Any IRVisitor::visitBxor(ifccParser::BxorContext *ctx)
{
    string lhs = any_cast<string>(visit(ctx->expr(0)));
    string rhs = any_cast<string>(visit(ctx->expr(1)));
    string dst = cfg->create_new_tempvar(TYPE_INT);
    cfg->current_bb->add_IRInstr(IRInstr::bxor, TYPE_INT, {lhs, rhs, dst});
    return dst;
}

antlrcpp::Any IRVisitor::visitBor(ifccParser::BorContext *ctx)
{
    string lhs = any_cast<string>(visit(ctx->expr(0)));
    string rhs = any_cast<string>(visit(ctx->expr(1)));
    string dst = cfg->create_new_tempvar(TYPE_INT);
    cfg->current_bb->add_IRInstr(IRInstr::bor, TYPE_INT, {lhs, rhs, dst});
    return dst;
}

antlrcpp::Any IRVisitor::visitNotExpr(ifccParser::NotExprContext *ctx)
{
    string val = any_cast<string>(visit(ctx->expr()));
    string zero = cfg->create_new_tempvar(TYPE_INT);
    string dst = cfg->create_new_tempvar(TYPE_INT);
    cfg->current_bb->add_IRInstr(IRInstr::ldconst, TYPE_INT, {zero, "0"});
    cfg->current_bb->add_IRInstr(IRInstr::cmp_eq, TYPE_INT, {val, zero, dst});
    return dst;
}

antlrcpp::Any IRVisitor::visitUnaryMinus(ifccParser::UnaryMinusContext *ctx)
{
    string val = any_cast<string>(visit(ctx->expr()));
    string zero = cfg->create_new_tempvar(TYPE_INT);
    string dst = cfg->create_new_tempvar(TYPE_INT);
    cfg->current_bb->add_IRInstr(IRInstr::ldconst, TYPE_INT, {zero, "0"});
    cfg->current_bb->add_IRInstr(IRInstr::sub, TYPE_INT, {zero, val, dst});
    return dst;
}

antlrcpp::Any IRVisitor::visitParens(ifccParser::ParensContext *ctx)
{
    return visit(ctx->expr());
}

antlrcpp::Any IRVisitor::visitConstExpr(ifccParser::ConstExprContext *ctx)
{
    string dst = cfg->create_new_tempvar(TYPE_INT);
    cfg->current_bb->add_IRInstr(IRInstr::ldconst, TYPE_INT, {dst, ctx->CONST()->getText()});
    return dst;
}

antlrcpp::Any IRVisitor::visitVarExpr(ifccParser::VarExprContext *ctx)
{
    return ctx->VAR()->getText();
}

antlrcpp::Any IRVisitor::visitFuncCall(ifccParser::FuncCallContext *ctx)
{
    vector<string> params;
    params.push_back(ctx->VAR()->getText());

    string dst = cfg->create_new_tempvar(TYPE_INT);
    params.push_back(dst);

    if (ctx->arg_list() != nullptr)
    {
        for (auto e : ctx->arg_list()->expr())
        {
            params.push_back(any_cast<string>(visit(e)));
        }
    }

    cfg->current_bb->add_IRInstr(IRInstr::call, TYPE_INT, params);
    return dst;
}

antlrcpp::Any IRVisitor::visitIf_stmt(ifccParser::If_stmtContext *ctx)
{
    string cond = any_cast<string>(visit(ctx->expr()));

    BasicBlock *thenBB  = new BasicBlock(cfg, cfg->new_BB_name());
    BasicBlock *afterBB = new BasicBlock(cfg, cfg->new_BB_name());
    cfg->add_bb(thenBB);
    cfg->add_bb(afterBB);

    cfg->current_bb->test_var_name = cond;

    if (ctx->block().size() > 1)
    {
        BasicBlock *elseBB = new BasicBlock(cfg, cfg->new_BB_name());
        cfg->add_bb(elseBB);

        cfg->current_bb->exit_true  = thenBB;
        cfg->current_bb->exit_false = elseBB;

        // then
        cfg->current_bb = thenBB;
        visit(ctx->block(0));
        BasicBlock* lastThenBB = cfg->current_bb; // capture après visite
            lastThenBB->exit_true = afterBB;

        // else
        cfg->current_bb = elseBB;
        visit(ctx->block(1));
        BasicBlock* lastElseBB = cfg->current_bb; // capture après visite
            lastElseBB->exit_true = afterBB;
    }
    else
    {
        cfg->current_bb->exit_true  = thenBB;
        cfg->current_bb->exit_false = afterBB;

        // then
        cfg->current_bb = thenBB;
        visit(ctx->block(0));
        BasicBlock* lastThenBB = cfg->current_bb; // capture après visite
            lastThenBB->exit_true = afterBB;
    }

    cfg->current_bb = afterBB;
    return 0;
}

antlrcpp::Any IRVisitor::visitBlock(ifccParser::BlockContext *ctx)
{
    for (auto s : ctx->stmt())
    {
        visit(s);
    }
    return 0;
}

antlrcpp::Any IRVisitor::visitWhile_stmt(ifccParser::While_stmtContext *ctx)
{
    BasicBlock *condBB  = new BasicBlock(cfg, cfg->new_BB_name());
    BasicBlock *bodyBB  = new BasicBlock(cfg, cfg->new_BB_name());
    BasicBlock *afterBB = new BasicBlock(cfg, cfg->new_BB_name());

    cfg->add_bb(condBB);
    cfg->add_bb(bodyBB);
    cfg->add_bb(afterBB);

    cfg->current_bb->exit_true = condBB;

    cfg->current_bb = condBB;
    string cond = any_cast<string>(visit(ctx->expr()));
    condBB->test_var_name = cond;
    condBB->exit_true  = bodyBB;
    condBB->exit_false = afterBB;

    breakStack.push_back(afterBB);
    continueStack.push_back(condBB);

    cfg->current_bb = bodyBB;
    visit(ctx->block());


    if (cfg->current_bb->exit_true == nullptr &&
        cfg->current_bb->exit_false == nullptr)
    {
        cfg->current_bb->exit_true = condBB; 
    }

    breakStack.pop_back();
    continueStack.pop_back();

    cfg->current_bb = afterBB;
    return 0;
}

antlrcpp::Any IRVisitor::visitBreak_stmt(ifccParser::Break_stmtContext *ctx)
{
    cfg->current_bb->exit_true = breakStack.back();

    BasicBlock *dead = new BasicBlock(cfg, cfg->new_BB_name());
    cfg->add_bb(dead);
    cfg->current_bb = dead;
    return 0;
}

antlrcpp::Any IRVisitor::visitContinue_stmt(ifccParser::Continue_stmtContext *ctx)
{
    cfg->current_bb->exit_true = continueStack.back();

    BasicBlock *dead = new BasicBlock(cfg, cfg->new_BB_name());
    cfg->add_bb(dead);
    cfg->current_bb = dead;
    return 0;
}

antlrcpp::Any IRVisitor::visitSwitch_stmt(ifccParser::Switch_stmtContext *ctx)
{
    // evaluate the switch expression
    string switchVal = any_cast<string>(visit(ctx->expr()));

    BasicBlock *afterBB = new BasicBlock(cfg, cfg->new_BB_name());
    cfg->add_bb(afterBB);

    // break inside any case jumps to afterBB
    breakStack.push_back(afterBB);

    auto cases   = ctx->case_clause();
    auto defClause = ctx->default_clause(); // nullable

    // create all body blocks upfront
    vector<BasicBlock*> bodyBlocks;
    for (int i = 0; i < (int)cases.size(); i++)
    {
        BasicBlock *bb = new BasicBlock(cfg, cfg->new_BB_name());
        cfg->add_bb(bb);
        bodyBlocks.push_back(bb);
    }

    BasicBlock *defaultBB = nullptr;
    if (defClause != nullptr)
    {
        defaultBB = new BasicBlock(cfg, cfg->new_BB_name());
        cfg->add_bb(defaultBB);
    }

    // chain of comparisons from current block
    for (int i = 0; i < (int)cases.size(); i++)
    {
        string constVal = cases[i]->CONST()->getText();

        // cmp switchVal == constVal
        string cmp = cfg->create_new_tempvar(TYPE_INT);
        string k   = cfg->create_new_tempvar(TYPE_INT);
        cfg->current_bb->add_IRInstr(IRInstr::ldconst, TYPE_INT, {k, constVal});
        cfg->current_bb->add_IRInstr(IRInstr::cmp_eq,  TYPE_INT, {switchVal, k, cmp});

        cfg->current_bb->test_var_name = cmp;
        cfg->current_bb->exit_true  = bodyBlocks[i];

        // false → next comparison block
        BasicBlock *nextCmp = new BasicBlock(cfg, cfg->new_BB_name());
        cfg->add_bb(nextCmp);
        cfg->current_bb->exit_false = nextCmp;
        cfg->current_bb = nextCmp;
    }

    // last comparison block: jump to default or after
    cfg->current_bb->exit_true = (defaultBB != nullptr) ? defaultBB : afterBB;

    // generate each case body
    for (int i = 0; i < (int)cases.size(); i++)
    {
        cfg->current_bb = bodyBlocks[i];
        for (auto s : cases[i]->stmt())
            visit(s);

        // fallthrough: if no break was hit, go to next case body (or afterBB)
        if (cfg->current_bb->exit_true  == nullptr &&
            cfg->current_bb->exit_false == nullptr)
        {
            if (i + 1 < (int)bodyBlocks.size())
                cfg->current_bb->exit_true = bodyBlocks[i + 1];
            else if (defaultBB != nullptr)
                cfg->current_bb->exit_true = defaultBB;
            else
                cfg->current_bb->exit_true = afterBB;
        }
    }

    // generate default body
    if (defaultBB != nullptr)
    {
        cfg->current_bb = defaultBB;
        for (auto s : defClause->stmt())
            visit(s);

        if (cfg->current_bb->exit_true  == nullptr &&
            cfg->current_bb->exit_false == nullptr)
        {
            cfg->current_bb->exit_true = afterBB;
        }
    }

    breakStack.pop_back();
    cfg->current_bb = afterBB;
    return 0;
}