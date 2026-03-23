#include "IRVisitor.h"
#include "IR.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

IRVisitor::IRVisitor() = default;

antlrcpp::Any IRVisitor::visitProg(ifccParser::ProgContext *ctx)
{
    for (auto f : ctx->function_def())
    {
        string name = f->VAR()->getText();
        if (functionArity.count(name))
        {
            cerr << "error: function '" << name << "' declared more than once\n";
            success = false;
            continue;
        }

        int arity = 0;
        if (f->param_list() != nullptr)
        {
            arity = static_cast<int>(f->param_list()->param().size());
        }
        functionArity[name] = arity;
    }

    if (!functionArity.count("main"))
    {
        cerr << "error: missing required 'main' function\n";
        success = false;
        return 0;
    }

    for (auto f : ctx->function_def())
    {
        visit(f);
    }

    return 0;
}

antlrcpp::Any IRVisitor::visitFunction_def(ifccParser::Function_defContext *ctx)
{
    currentFunction = ctx->VAR()->getText();
    cfg = new CFG(currentFunction);
    currentSymbols.clear();

    vector<string> params;
    if (ctx->param_list() != nullptr)
    {
        for (auto p : ctx->param_list()->param())
        {
            string pname = p->VAR()->getText();
            if (currentSymbols.count(pname))
            {
                cerr << "error: parameter '" << pname << "' declared more than once in function '"
                     << currentFunction << "'\n";
                success = false;
                continue;
            }
            currentSymbols.insert(pname);
            cfg->add_to_symbol_table(pname, TYPE_INT);
            params.push_back(pname);
        }
    }
    cfg->set_param_order(params);

    BasicBlock *entry = new BasicBlock(cfg, currentFunction + "_entry");
    cfg->add_bb(entry);
    cfg->current_bb = entry;

    returnBB = new BasicBlock(cfg, currentFunction + "_ret");
    cfg->add_bb(returnBB);

    visit(ctx->block());

    if (cfg->current_bb->exit_true == nullptr && cfg->current_bb->exit_false == nullptr)
    {
        string zero = cfg->create_new_tempvar(TYPE_INT);
        cfg->current_bb->add_IRInstr(IRInstr::ldconst, TYPE_INT, {zero, "0"});
        cfg->current_bb->add_IRInstr(IRInstr::ret, TYPE_INT, {zero});
        cfg->current_bb->exit_true = returnBB;
    }

    if (success)
    {
        cfg->gen_asm(std::cout);
    }
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
        string lhs = v->VAR()->getText();
        if (currentSymbols.count(lhs))
        {
            cerr << "error: variable '" << lhs << "' declared more than once in function '"
                 << currentFunction << "'\n";
            success = false;
            continue;
        }

        currentSymbols.insert(lhs);
        cfg->add_to_symbol_table(lhs, TYPE_INT);

        if (v->expr() != nullptr)
        {
            string rhs = any_cast<string>(visit(v->expr()));
            cfg->current_bb->add_IRInstr(IRInstr::copy, TYPE_INT, {rhs, lhs});
        }
    }
    return 0;
}

antlrcpp::Any IRVisitor::visitAssign_stmt(ifccParser::Assign_stmtContext *ctx)
{
    string lhs = ctx->VAR()->getText();
    if (!cfg->has_symbol(lhs))
    {
        cerr << "error: variable '" << lhs << "' used before declaration in function '"
             << currentFunction << "'\n";
        success = false;
        return 0;
    }

    string rhs = any_cast<string>(visit(ctx->expr()));
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
    string name = ctx->VAR()->getText();
    if (!cfg->has_symbol(name))
    {
        cerr << "error: variable '" << name << "' used before declaration in function '"
             << currentFunction << "'\n";
        success = false;
        string zero = cfg->create_new_tempvar(TYPE_INT);
        cfg->current_bb->add_IRInstr(IRInstr::ldconst, TYPE_INT, {zero, "0"});
        return zero;
    }
    return name;
}

antlrcpp::Any IRVisitor::visitFuncCall(ifccParser::FuncCallContext *ctx)
{
    string fname = ctx->VAR()->getText();
    int argCount = 0;
    if (ctx->arg_list() != nullptr)
    {
        argCount = static_cast<int>(ctx->arg_list()->expr().size());
    }

    if (!functionArity.count(fname))
    {
        cerr << "error: call to unknown function '" << fname << "'\n";
        success = false;
    }
    else if (functionArity[fname] != argCount)
    {
        cerr << "error: function '" << fname << "' expects " << functionArity[fname]
             << " argument(s), got " << argCount << "\n";
        success = false;
    }

    if (argCount > 6)
    {
        cerr << "error: functions with more than 6 arguments are not supported yet\n";
        success = false;
    }

    vector<string> params;
    params.push_back(fname);

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

        cfg->current_bb = thenBB;
        visit(ctx->block(0));
        BasicBlock *lastThenBB = cfg->current_bb;
        if (lastThenBB->exit_true == nullptr && lastThenBB->exit_false == nullptr)
        {
            lastThenBB->exit_true = afterBB;
        }

        cfg->current_bb = elseBB;
        visit(ctx->block(1));
        BasicBlock *lastElseBB = cfg->current_bb;
        if (lastElseBB->exit_true == nullptr && lastElseBB->exit_false == nullptr)
        {
            lastElseBB->exit_true = afterBB;
        }
    }
    else
    {
        cfg->current_bb->exit_true  = thenBB;
        cfg->current_bb->exit_false = afterBB;

        cfg->current_bb = thenBB;
        visit(ctx->block(0));
        BasicBlock *lastThenBB = cfg->current_bb;
        if (lastThenBB->exit_true == nullptr && lastThenBB->exit_false == nullptr)
        {
            lastThenBB->exit_true = afterBB;
        }
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
    BasicBlock *condBB = new BasicBlock(cfg, cfg->new_BB_name());
    BasicBlock *bodyBB = new BasicBlock(cfg, cfg->new_BB_name());
    BasicBlock *afterBB = new BasicBlock(cfg, cfg->new_BB_name());

    cfg->add_bb(condBB);
    cfg->add_bb(bodyBB);
    cfg->add_bb(afterBB);

    cfg->current_bb->exit_true = condBB;

    cfg->current_bb = condBB;
    string cond = any_cast<string>(visit(ctx->expr()));
    condBB->test_var_name = cond;
    condBB->exit_true = bodyBB;
    condBB->exit_false = afterBB;

    cfg->current_bb = bodyBB;
    visit(ctx->block());
    if (cfg->current_bb->exit_true == nullptr && cfg->current_bb->exit_false == nullptr)
    {
        cfg->current_bb->exit_true = condBB;
    }

    cfg->current_bb = afterBB;
    return 0;
}
