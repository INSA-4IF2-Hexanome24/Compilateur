#include "IRVisitor.h"
#include "IR.h"

#include <iostream>
#include <string>
#include <vector>

using std::string;
using std::vector;

using namespace std;

bool IRVisitor::tryFoldConst(ifccParser::ExprContext *ctx, int &val)
{
    if (auto *c = dynamic_cast<ifccParser::ConstExprContext *>(ctx))
    {
        val = std::stoi(c->CONST()->getText());
        return true;
    }
    if (auto *u = dynamic_cast<ifccParser::UnaryMinusContext *>(ctx))
    {
        int v;
        if (tryFoldConst(u->expr(), v)) { val = -v; return true; }
    }
    if (auto *p = dynamic_cast<ifccParser::ParensContext *>(ctx))
        return tryFoldConst(p->expr(), val);
    if (auto *e = dynamic_cast<ifccParser::PlusminusContext *>(ctx))
    {
        int l, r;
        if (tryFoldConst(e->expr(0), l) && tryFoldConst(e->expr(1), r))
        {
            val = (e->OP->getText() == "+") ? l + r : l - r;
            return true;
        }
    }
    if (auto *e = dynamic_cast<ifccParser::MultdivmodContext *>(ctx))
    {
        int l, r;
        if (tryFoldConst(e->expr(0), l) && tryFoldConst(e->expr(1), r))
        {
            string op = e->OP->getText();
            if (op == "*") val = l * r;
            else if (op == "/" && r != 0) val = l / r;
            else if (op == "%" && r != 0) val = l % r;
            else return false;
            return true;
        }
    }
    if (auto *e = dynamic_cast<ifccParser::LtgtContext *>(ctx))
    {
        int l, r;
        if (tryFoldConst(e->expr(0), l) && tryFoldConst(e->expr(1), r))
        {
            val = (e->OP->getText() == "<") ? (l < r) : (l > r);
            return true;
        }
    }
    if (auto *e = dynamic_cast<ifccParser::EqneqContext *>(ctx))
    {
        int l, r;
        if (tryFoldConst(e->expr(0), l) && tryFoldConst(e->expr(1), r))
        {
            val = (e->OP->getText() == "==") ? (l == r) : (l != r);
            return true;
        }
    }
    if (auto *e = dynamic_cast<ifccParser::BandContext *>(ctx))
    {
        int l, r;
        if (tryFoldConst(e->expr(0), l) && tryFoldConst(e->expr(1), r))
        { val = l & r; return true; }
    }
    if (auto *e = dynamic_cast<ifccParser::BxorContext *>(ctx))
    {
        int l, r;
        if (tryFoldConst(e->expr(0), l) && tryFoldConst(e->expr(1), r))
        { val = l ^ r; return true; }
    }
    if (auto *e = dynamic_cast<ifccParser::BorContext *>(ctx))
    {
        int l, r;
        if (tryFoldConst(e->expr(0), l) && tryFoldConst(e->expr(1), r))
        { val = l | r; return true; }
    }
    return false;
}

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
    int foldedVal;
    if (tryFoldConst(ctx, foldedVal))
    {
        string dst = cfg->create_new_tempvar(TYPE_INT);
        cfg->current_bb->add_IRInstr(
            IRInstr::ldconst, TYPE_INT, {dst, std::to_string(foldedVal)});
        return dst;
    }

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

    int foldedVal;
    if (tryFoldConst(ctx, foldedVal))
    {
        string dst = cfg->create_new_tempvar(TYPE_INT);
        cfg->current_bb->add_IRInstr(
            IRInstr::ldconst, TYPE_INT, {dst, std::to_string(foldedVal)});
        return dst;
    }
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
    int foldedVal;
    if (tryFoldConst(ctx, foldedVal))
    {
        string dst = cfg->create_new_tempvar(TYPE_INT);
        cfg->current_bb->add_IRInstr(
            IRInstr::ldconst, TYPE_INT, {dst, std::to_string(foldedVal)});
        return dst;
    }

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
    int foldedVal;
    if (tryFoldConst(ctx, foldedVal))
    {
        string dst = cfg->create_new_tempvar(TYPE_INT);
        cfg->current_bb->add_IRInstr(
            IRInstr::ldconst, TYPE_INT, {dst, std::to_string(foldedVal)});
        return dst;
    }

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
    int foldedVal;
    if (tryFoldConst(ctx, foldedVal))
    {
        string dst = cfg->create_new_tempvar(TYPE_INT);
        cfg->current_bb->add_IRInstr(
            IRInstr::ldconst, TYPE_INT, {dst, std::to_string(foldedVal)});
        return dst;
    }

    string lhs = any_cast<string>(visit(ctx->expr(0)));
    string rhs = any_cast<string>(visit(ctx->expr(1)));
    string dst = cfg->create_new_tempvar(TYPE_INT);
    cfg->current_bb->add_IRInstr(IRInstr::band, TYPE_INT, {lhs, rhs, dst});
    return dst;
}

antlrcpp::Any IRVisitor::visitBxor(ifccParser::BxorContext *ctx)
{
    int foldedVal;
    if (tryFoldConst(ctx, foldedVal))
    {
        string dst = cfg->create_new_tempvar(TYPE_INT);
        cfg->current_bb->add_IRInstr(
            IRInstr::ldconst, TYPE_INT, {dst, std::to_string(foldedVal)});
        return dst;
    }

    string lhs = any_cast<string>(visit(ctx->expr(0)));
    string rhs = any_cast<string>(visit(ctx->expr(1)));
    string dst = cfg->create_new_tempvar(TYPE_INT);
    cfg->current_bb->add_IRInstr(IRInstr::bxor, TYPE_INT, {lhs, rhs, dst});
    return dst;
}

antlrcpp::Any IRVisitor::visitBor(ifccParser::BorContext *ctx)
{
    int foldedVal;
    if (tryFoldConst(ctx, foldedVal))
    {
        string dst = cfg->create_new_tempvar(TYPE_INT);
        cfg->current_bb->add_IRInstr(
            IRInstr::ldconst, TYPE_INT, {dst, std::to_string(foldedVal)});
        return dst;
    }

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

antlrcpp::Any IRVisitor::visitBreakStmt(ifccParser::BreakStmtContext *ctx)
{
    if (!breakTargets.empty())
    {
        cfg->current_bb->exit_true = breakTargets.back();
    }
    // dead block after break
    BasicBlock *dead = new BasicBlock(cfg, cfg->new_BB_name());
    cfg->add_bb(dead);
    cfg->current_bb = dead;
    return 0;
}

antlrcpp::Any IRVisitor::visitContinueStmt(ifccParser::ContinueStmtContext *ctx)
{
    if (!continueTargets.empty())
    {
        cfg->current_bb->exit_true = continueTargets.back();
    }
    BasicBlock *dead = new BasicBlock(cfg, cfg->new_BB_name());
    cfg->add_bb(dead);
    cfg->current_bb = dead;
    return 0;
}

antlrcpp::Any IRVisitor::visitSwitchStmt(ifccParser::SwitchStmtContext *ctx)
{
    string switchVal = any_cast<string>(visit(ctx->expr()));

    auto *sw = ctx->switch_block();
    int  n   = static_cast<int>(sw->CONST().size());

    // afterBB is where break and fall-through go
    BasicBlock *afterBB = new BasicBlock(cfg, cfg->new_BB_name());
    cfg->add_bb(afterBB);
    breakTargets.push_back(afterBB);

    // Build one BB per case body + one for default (or afterBB)
    vector<BasicBlock *> caseBBs;
    for (int i = 0; i < n; i++)
    {
        BasicBlock *bb = new BasicBlock(cfg, cfg->new_BB_name());
        cfg->add_bb(bb);
        caseBBs.push_back(bb);
    }

    BasicBlock *defaultBB = afterBB;
    if (sw->DEFAULT() != nullptr)
    {
        defaultBB = new BasicBlock(cfg, cfg->new_BB_name());
        cfg->add_bb(defaultBB);
    }

    // Emit comparison chain: for each case emit cmp + conditional jump
    for (int i = 0; i < n; i++)
    {
        string caseVal = sw->CONST(i)->getText();
        string cst     = cfg->create_new_tempvar(TYPE_INT);
        string cmp     = cfg->create_new_tempvar(TYPE_INT);

        cfg->current_bb->add_IRInstr(IRInstr::ldconst, TYPE_INT, {cst, caseVal});
        cfg->current_bb->add_IRInstr(IRInstr::cmp_eq,  TYPE_INT, {switchVal, cst, cmp});

        cfg->current_bb->test_var_name = cmp;
        cfg->current_bb->exit_true     = caseBBs[i];

        // Next comparison goes into a fresh BB
        BasicBlock *nextCmp = new BasicBlock(cfg, cfg->new_BB_name());
        cfg->add_bb(nextCmp);
        cfg->current_bb->exit_false = nextCmp;
        cfg->current_bb             = nextCmp;
    }

    // Remaining current_bb falls through to default (or after)
    cfg->current_bb->exit_true = defaultBB;

    // Emit each case body; fall-through links to the next case body
    for (int i = 0; i < n; i++)
    {
        cfg->current_bb = caseBBs[i];
        visit(sw->block(i));
        // fall-through to next case body (or defaultBB if last)
        if (cfg->current_bb->exit_true == nullptr &&
            cfg->current_bb->exit_false == nullptr)
        {
            BasicBlock *next = (i + 1 < n) ? caseBBs[i + 1] : defaultBB;
            cfg->current_bb->exit_true = next;
        }
    }

    // Emit default body if present
    if (sw->DEFAULT() != nullptr)
    {
        cfg->current_bb = defaultBB;
        visit(sw->block(n)); // default block is after all case blocks
        if (cfg->current_bb->exit_true == nullptr &&
            cfg->current_bb->exit_false == nullptr)
        {
            cfg->current_bb->exit_true = afterBB;
        }
    }

    breakTargets.pop_back();
    cfg->current_bb = afterBB;
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

    breakTargets.push_back(afterBB);    // <-- added for switch case
    continueTargets.push_back(condBB);  // <-- added

    cfg->current_bb = bodyBB;
    visit(ctx->block());
    if (bodyBB->exit_true == nullptr && bodyBB->exit_false == nullptr)
    {
        bodyBB->exit_true = condBB;
    }

    breakTargets.pop_back();    // <-- added
    continueTargets.pop_back(); // <-- added

    cfg->current_bb = afterBB;
    return 0;
}