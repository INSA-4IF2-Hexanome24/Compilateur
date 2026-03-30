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
            string lhs = currentPrefix() + v->VAR()->getText();
            cfg->current_bb->add_IRInstr(IRInstr::copy, TYPE_INT, {rhs, lhs});
        }
    }
    return 0;
}

antlrcpp::Any IRVisitor::visitSimpleAssign(ifccParser::SimpleAssignContext *ctx)
{
    string rhs = any_cast<string>(visit(ctx->expr()));
    string lhs = ctx->VAR()->getText();
    lhs = resolveVar(lhs);
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
    std::string name = ctx->VAR()->getText();
    return resolveVar(name);
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

    BasicBlock *condBB = cfg->current_bb;
    BasicBlock *thenBB = new BasicBlock(cfg, cfg->new_BB_name());
    cfg->add_bb(thenBB);

    BasicBlock *elseBB = nullptr;
    BasicBlock *afterBB = nullptr;

    condBB->test_var_name = cond;

    if (ctx->block().size() > 1)
    {
        elseBB = new BasicBlock(cfg, cfg->new_BB_name());
        afterBB = new BasicBlock(cfg, cfg->new_BB_name());

        cfg->add_bb(elseBB);
        cfg->add_bb(afterBB);

        condBB->exit_true = thenBB;
        condBB->exit_false = elseBB;

        // then
        cfg->current_bb = thenBB;
        visit(ctx->block(0));
        if (cfg->current_bb->exit_true == nullptr &&
            cfg->current_bb->exit_false == nullptr)
        {
            cfg->current_bb->exit_true = afterBB;
        }

        // else
        cfg->current_bb = elseBB;
        visit(ctx->block(1));
        if (cfg->current_bb->exit_true == nullptr &&
            cfg->current_bb->exit_false == nullptr)
        {
            cfg->current_bb->exit_true = afterBB;
        }
    }
    else
    {
        afterBB = new BasicBlock(cfg, cfg->new_BB_name());
        cfg->add_bb(afterBB);

        condBB->exit_true = thenBB;
        condBB->exit_false = afterBB;

        cfg->current_bb = thenBB;
        visit(ctx->block(0));
        if (cfg->current_bb->exit_true == nullptr &&
            cfg->current_bb->exit_false == nullptr)
        {
            cfg->current_bb->exit_true = afterBB;
        }
    }

    cfg->current_bb = afterBB;
    return 0;
}

antlrcpp::Any IRVisitor::visitBlock(ifccParser::BlockContext *ctx)
{
    pair<int, int> *parent = nullptr; // pointeur, null si pas de parent
    pair<int, int> enfant;

    if (!scopeStack.empty())
    {
        parent = &scopeStack.back();
        enfant = {parent->second, 0};
    }
    else
    {
        enfant = {0, 0};
    }

    scopeStack.push_back(enfant);

    for (auto s : ctx->stmt())
    {
        visit(s);
    }

    scopeStack.pop_back();

    if (parent != nullptr)
    {
        parent->second++;
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
    if (bodyBB->exit_true == nullptr && bodyBB->exit_false == nullptr)
    {
        bodyBB->exit_true = condBB;
    }

    cfg->current_bb = afterBB;
    return 0;
}

antlrcpp::Any IRVisitor::visitTab(ifccParser::TabContext *ctx)
{
    std::string nomTab = ctx->VAR()->getText();
    int taille = stoi(ctx->CONST()->getText()); // stoi = string to int
    cfg->add_tableau_to_symbol_table(nomTab, TYPE_INT, taille);
    return 0;
}

antlrcpp::Any IRVisitor::visitArrayRead(ifccParser::ArrayReadContext *ctx)
{
    std::string nomTab = ctx->VAR()->getText();
    std::string indice = any_cast<string>(visit(ctx->expr()));
    std::string addrTab = cfg->create_new_tempvar(TYPE_PTR);
    cfg->current_bb->add_IRInstr(IRInstr::addrof, TYPE_PTR, {addrTab, nomTab}); //mets l'adresse du tableau dans addrTab
    
    std::string quatre = cfg->create_new_tempvar(TYPE_INT);
    cfg->current_bb->add_IRInstr(IRInstr::ldconst, TYPE_INT, {quatre, "4"}); // charge 4

    std::string quatrei = cfg->create_new_tempvar(TYPE_INT);
    cfg->current_bb->add_IRInstr(IRInstr::mul,TYPE_INT,{quatre, indice, quatrei}); // charge 4*i

    std::string adresseFinal = cfg->create_new_tempvar(TYPE_PTR);
    cfg->current_bb->add_IRInstr(IRInstr::add,TYPE_INT,{quatrei, addrTab, adresseFinal}); // charge 4*i + adresse tableau

    std::string dst = cfg->create_new_tempvar(TYPE_INT);
    cfg->current_bb->add_IRInstr(IRInstr::rmem,TYPE_INT,{dst, adresseFinal});

    return dst;
}

antlrcpp::Any IRVisitor::visitArrayWrite(ifccParser::ArrayWriteContext *ctx)
{
    std::string nomTab = ctx->VAR()->getText();
    std::string indice = any_cast<string>(visit(ctx->expr(0)));

    std::string rvalue = any_cast<string>(visit(ctx->expr(1)));

    std::string addrTab = cfg->create_new_tempvar(TYPE_PTR);
    cfg->current_bb->add_IRInstr(IRInstr::addrof, TYPE_PTR, {addrTab, nomTab}); //mets l'adresse du tableau dans addrTab
    
    std::string quatre = cfg->create_new_tempvar(TYPE_INT);
    cfg->current_bb->add_IRInstr(IRInstr::ldconst, TYPE_INT, {quatre, "4"}); // charge 4

    std::string quatrei = cfg->create_new_tempvar(TYPE_INT);
    cfg->current_bb->add_IRInstr(IRInstr::mul,TYPE_INT,{quatre, indice, quatrei}); // charge 4*i

    std::string adresseFinal = cfg->create_new_tempvar(TYPE_PTR);
    cfg->current_bb->add_IRInstr(IRInstr::add,TYPE_INT,{quatrei, addrTab, adresseFinal}); // charge 4*i + adresse tableau

    cfg->current_bb->add_IRInstr(IRInstr::wmem,TYPE_INT,{rvalue,adresseFinal});

    return 0;
}

std::string IRVisitor::currentPrefix()
{
    std::vector<pair<int, int>>::iterator it;
    std::string resultat = "";
    for (it = scopeStack.begin(); it != scopeStack.end(); it++)
    {
        resultat += std::to_string(it->first) + "-";
    }
    return resultat;
}

std::string IRVisitor::resolveVar(std::string var)
{
    // essaie du scope le plus profond au plus global
    for (int i = scopeStack.size(); i >= 0; i--)
    {
        // construire le préfixe avec les i premiers éléments de scopeStack
        std::string prefix = "";
        for (int j = 0; j < i; j++)
        {
            prefix += std::to_string(scopeStack[j].first) + "-";
        }
        std::string candidate = prefix + var;
        if (cfg->has_symbol(candidate))
            return candidate;
    }
    return var; // pas trouvé
}