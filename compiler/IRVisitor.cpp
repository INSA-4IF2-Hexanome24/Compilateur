#include "IRVisitor.h"
#include "IR.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

IRVisitor::IRVisitor() = default;

Type IRVisitor::declaredType(const string &name) const
{
    return cfg->get_var_type(name);
}

bool IRVisitor::isDeclaredInScope(const string &name) const
{
    for (auto it = scopeStack.rbegin(); it != scopeStack.rend(); ++it)
    {
        if (it->count(name))
        {
            return true;
        }
    }
    return false;
}

antlrcpp::Any IRVisitor::visitProg(ifccParser::ProgContext *ctx)
{
    // Predeclared external functions.
    functionArity["getchar"] = 0;
    functionArity["putchar"] = 1;
    functionParamTypes["getchar"] = {};
    functionParamTypes["putchar"] = {TYPE_INT};

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

        vector<Type> paramTypes;
        if (f->param_list() != nullptr)
        {
            for (auto p : f->param_list()->param())
            {
                bool isPtr = p->getText().find('*') != string::npos;
                paramTypes.push_back(isPtr ? TYPE_PTR : TYPE_INT);
            }
        }
        functionParamTypes[name] = paramTypes;
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
    scopeStack.clear();
    scopeStack.push_back({});

    vector<string> params;
    if (ctx->param_list() != nullptr)
    {
        for (auto p : ctx->param_list()->param())
        {
            string pname = p->VAR()->getText();
            Type ptype = (p->getText().find('*') != string::npos) ? TYPE_PTR : TYPE_INT;
            if (currentSymbols.count(pname))
            {
                cerr << "error: parameter '" << pname << "' declared more than once in function '"
                     << currentFunction << "'\n";
                success = false;
                continue;
            }
            currentSymbols.insert(pname);
            scopeStack.back().insert(pname);
            cfg->add_to_symbol_table(pname, ptype);
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
        Type lhsType = (v->getText().rfind("*", 0) == 0) ? TYPE_PTR : TYPE_INT;
        if (!scopeStack.empty() && scopeStack.back().count(lhs))
        {
            cerr << "error: variable '" << lhs << "' declared more than once in function '"
                 << currentFunction << "'\n";
            success = false;
            continue;
        }

        currentSymbols.insert(lhs);
        if (!scopeStack.empty())
        {
            scopeStack.back().insert(lhs);
        }
        cfg->add_to_symbol_table(lhs, lhsType);

        if (v->expr() != nullptr)
        {
            string rhs = any_cast<string>(visit(v->expr()));
            Type rhsType = cfg->get_var_type(rhs);
            if (lhsType != rhsType)
            {
                cerr << "error: type mismatch in initialization of '" << lhs
                     << "' in function '" << currentFunction << "'\n";
                success = false;
            }
            else
            {
                cfg->current_bb->add_IRInstr(IRInstr::copy, lhsType, {rhs, lhs});
            }
        }
    }
    return 0;
}

antlrcpp::Any IRVisitor::visitAssignSimple_stmt(ifccParser::AssignSimple_stmtContext *ctx)
{
    string lhs = ctx->VAR()->getText();
    if (!isDeclaredInScope(lhs))
    {
        cerr << "error: variable '" << lhs << "' used before declaration in function '"
             << currentFunction << "'\n";
        success = false;
        return 0;
    }

    Type lhsType = declaredType(lhs);
    string rhs = any_cast<string>(visit(ctx->expr()));
    Type rhsType = cfg->get_var_type(rhs);
    if (lhsType != rhsType)
    {
        cerr << "error: type mismatch in assignment to '" << lhs
             << "' in function '" << currentFunction << "'\n";
        success = false;
        return 0;
    }

    cfg->current_bb->add_IRInstr(IRInstr::copy, lhsType, {rhs, lhs});
    return 0;
}

antlrcpp::Any IRVisitor::visitAddAssign_stmt(ifccParser::AddAssign_stmtContext *ctx)
{
    string lhs = ctx->VAR()->getText();

    if (!isDeclaredInScope(lhs))
    {
        cerr << "error: variable '" << lhs << "' used before declaration in function '"
             << currentFunction << "'\n";
        success = false;
        return 0;
    }

    Type lhsType = declaredType(lhs);
    string rhs = any_cast<string>(visit(ctx->expr()));
    Type rhsType = cfg->get_var_type(rhs);
    
    if (lhsType != rhsType)
    {
        cerr << "error: type mismatch in assignment to '" << lhs
             << "' in function '" << currentFunction << "'\n";
        success = false;
        return 0;
    }

    // a += b  =>  a = a + b
    string temp = cfg->create_new_tempvar(TYPE_INT);
    cfg->current_bb->add_IRInstr(IRInstr::add, TYPE_INT, {lhs, rhs, temp});
    cfg->current_bb->add_IRInstr(IRInstr::copy, lhsType, {temp, lhs});
    return 0;
}

antlrcpp::Any IRVisitor::visitMinusAssign_stmt(ifccParser::MinusAssign_stmtContext *ctx)
{
    string lhs = ctx->VAR()->getText();
    
    if (!isDeclaredInScope(lhs))
    {
        cerr << "error: variable '" << lhs << "' used before declaration in function '"
             << currentFunction << "'\n";
        success = false;
        return 0;
    }

    Type lhsType = declaredType(lhs);
    string rhs = any_cast<string>(visit(ctx->expr()));
    Type rhsType = cfg->get_var_type(rhs);
    
    if (lhsType != rhsType)
    {
        cerr << "error: type mismatch in assignment to '" << lhs
             << "' in function '" << currentFunction << "'\n";
        success = false;
        return 0;
    }

    // a -= b  =>  a = a - b
    string temp = cfg->create_new_tempvar(TYPE_INT);
    cfg->current_bb->add_IRInstr(IRInstr::sub, TYPE_INT, {lhs, rhs, temp});
    cfg->current_bb->add_IRInstr(IRInstr::copy, lhsType, {temp, lhs});
    return 0;
}

antlrcpp::Any IRVisitor::visitMultAssign_stmt(ifccParser::MultAssign_stmtContext *ctx)
{
    string lhs = ctx->VAR()->getText();
    
    if (!isDeclaredInScope(lhs))
    {
        cerr << "error: variable '" << lhs << "' used before declaration in function '"
             << currentFunction << "'\n";
        success = false;
        return 0;
    }

    Type lhsType = declaredType(lhs);
    string rhs = any_cast<string>(visit(ctx->expr()));
    Type rhsType = cfg->get_var_type(rhs);
    
    if (lhsType != rhsType)
    {
        cerr << "error: type mismatch in assignment to '" << lhs
             << "' in function '" << currentFunction << "'\n";
        success = false;
        return 0;
    }

    // a *= b  =>  a = a * b
    string temp = cfg->create_new_tempvar(TYPE_INT);
    cfg->current_bb->add_IRInstr(IRInstr::mul, TYPE_INT, {lhs, rhs, temp});
    cfg->current_bb->add_IRInstr(IRInstr::copy, lhsType, {temp, lhs});
    return 0;
}

antlrcpp::Any IRVisitor::visitDivAssign_stmt(ifccParser::DivAssign_stmtContext *ctx)
{
    string lhs = ctx->VAR()->getText();
    
    if (!isDeclaredInScope(lhs))
    {
        cerr << "error: variable '" << lhs << "' used before declaration in function '"
             << currentFunction << "'\n";
        success = false;
        return 0;
    }

    Type lhsType = declaredType(lhs);
    string rhs = any_cast<string>(visit(ctx->expr()));
    Type rhsType = cfg->get_var_type(rhs);
    
    if (lhsType != rhsType)
    {
        cerr << "error: type mismatch in assignment to '" << lhs
             << "' in function '" << currentFunction << "'\n";
        success = false;
        return 0;
    }

    // a /= b  =>  a = a / b
    string temp = cfg->create_new_tempvar(TYPE_INT);
    cfg->current_bb->add_IRInstr(IRInstr::div, TYPE_INT, {lhs, rhs, temp});
    cfg->current_bb->add_IRInstr(IRInstr::copy, lhsType, {temp, lhs});
    return 0;
}

antlrcpp::Any IRVisitor::visitPreMinus_stmt(ifccParser::PreMinus_stmtContext *ctx)
{
    string var = ctx->VAR()->getText();
    
    if (!isDeclaredInScope(var))
    {
        cerr << "error: variable '" << var << "' used before declaration in function '"
             << currentFunction << "'\n";
        success = false;
        return 0;
    }

    if (declaredType(var) != TYPE_INT)
    {
        cerr << "error: cannot decrement non-int variable '" << var
             << "' in function '" << currentFunction << "'\n";
        success = false;
        return 0;
    }

    // --a  =>  a = a - 1
    string one = cfg->create_new_tempvar(TYPE_INT);
    string temp = cfg->create_new_tempvar(TYPE_INT);
    cfg->current_bb->add_IRInstr(IRInstr::ldconst, TYPE_INT, {one, "1"});
    cfg->current_bb->add_IRInstr(IRInstr::sub, TYPE_INT, {var, one, temp});
    cfg->current_bb->add_IRInstr(IRInstr::copy, TYPE_INT, {temp, var});
    return 0;
}

antlrcpp::Any IRVisitor::visitPreAdd_stmt(ifccParser::PreAdd_stmtContext *ctx)
{
    string var = ctx->VAR()->getText();
    
    if (!isDeclaredInScope(var))
    {
        cerr << "error: variable '" << var << "' used before declaration in function '"
             << currentFunction << "'\n";
        success = false;
        return 0;
    }

    if (declaredType(var) != TYPE_INT)
    {
        cerr << "error: cannot increment non-int variable '" << var
             << "' in function '" << currentFunction << "'\n";
        success = false;
        return 0;
    }

    // ++a  =>  a = a + 1
    string one = cfg->create_new_tempvar(TYPE_INT);
    string temp = cfg->create_new_tempvar(TYPE_INT);
    cfg->current_bb->add_IRInstr(IRInstr::ldconst, TYPE_INT, {one, "1"});
    cfg->current_bb->add_IRInstr(IRInstr::add, TYPE_INT, {var, one, temp});
    cfg->current_bb->add_IRInstr(IRInstr::copy, TYPE_INT, {temp, var});
    return 0;
}

antlrcpp::Any IRVisitor::visitPostAdd_stmt(ifccParser::PostAdd_stmtContext *ctx)
{
    string var = ctx->VAR()->getText();
    
    if (!isDeclaredInScope(var))
    {
        cerr << "error: variable '" << var << "' used before declaration in function '"
             << currentFunction << "'\n";
        success = false;
        return 0;
    }

    if (declaredType(var) != TYPE_INT)
    {
        cerr << "error: cannot increment non-int variable '" << var
             << "' in function '" << currentFunction << "'\n";
        success = false;
        return 0;
    }

    // a++  =>  a = a + 1 (statement, value not used)
    string one = cfg->create_new_tempvar(TYPE_INT);
    string temp = cfg->create_new_tempvar(TYPE_INT);
    cfg->current_bb->add_IRInstr(IRInstr::ldconst, TYPE_INT, {one, "1"});
    cfg->current_bb->add_IRInstr(IRInstr::add, TYPE_INT, {var, one, temp});
    cfg->current_bb->add_IRInstr(IRInstr::copy, TYPE_INT, {temp, var});
    return 0;
}

antlrcpp::Any IRVisitor::visitPostMinus_stmt(ifccParser::PostMinus_stmtContext *ctx)
{
    string var = ctx->VAR()->getText();
    
    if (!isDeclaredInScope(var))
    {
        cerr << "error: variable '" << var << "' used before declaration in function '"
             << currentFunction << "'\n";
        success = false;
        return 0;
    }

    if (declaredType(var) != TYPE_INT)
    {
        cerr << "error: cannot decrement non-int variable '" << var
             << "' in function '" << currentFunction << "'\n";
        success = false;
        return 0;
    }
    string one = cfg->create_new_tempvar(TYPE_INT);
    string temp = cfg->create_new_tempvar(TYPE_INT);
    cfg->current_bb->add_IRInstr(IRInstr::ldconst, TYPE_INT, {one, "1"});
    cfg->current_bb->add_IRInstr(IRInstr::sub, TYPE_INT, {var, one, temp});
    cfg->current_bb->add_IRInstr(IRInstr::copy, TYPE_INT, {temp, var});
    return 0;
}

antlrcpp::Any IRVisitor::visitPtr_assign_stmt(ifccParser::Ptr_assign_stmtContext *ctx)
{
    string ptrName = ctx->VAR()->getText();
    if (!isDeclaredInScope(ptrName))
    {
        cerr << "error: variable '" << ptrName << "' used before declaration in function '"
             << currentFunction << "'\n";
        success = false;
        return 0;
    }

    if (declaredType(ptrName) != TYPE_PTR)
    {
        cerr << "error: cannot dereference non-pointer '" << ptrName
             << "' in function '" << currentFunction << "'\n";
        success = false;
        return 0;
    }

    string rhs = any_cast<string>(visit(ctx->expr()));
    if (cfg->get_var_type(rhs) != TYPE_INT)
    {
        cerr << "error: cannot store non-int through pointer '" << ptrName
             << "' in function '" << currentFunction << "'\n";
        success = false;
        return 0;
    }

    cfg->current_bb->add_IRInstr(IRInstr::wmem, TYPE_INT, {ptrName, rhs});
    return 0;
}

antlrcpp::Any IRVisitor::visitReturn_stmt(ifccParser::Return_stmtContext *ctx)
{
    string retVal;
    if (ctx->expr() != nullptr)
    {
        retVal = any_cast<string>(visit(ctx->expr()));
        if (cfg->get_var_type(retVal) != TYPE_INT)
        {
            cerr << "error: function '" << currentFunction << "' returns int, got pointer\n";
            success = false;
        }
    }
    else
    {
        retVal = cfg->create_new_tempvar(TYPE_INT);
        cfg->current_bb->add_IRInstr(IRInstr::ldconst, TYPE_INT, {retVal, "0"});
    }
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


antlrcpp::Any IRVisitor::visitAndCond(ifccParser::AndCondContext *ctx)
{
    string lhs = any_cast<string>(visit(ctx->expr(0)));
    
    // Crear bloques
    BasicBlock *evalRhs_BB = new BasicBlock(cfg, cfg->new_BB_name());
    BasicBlock *after_BB = new BasicBlock(cfg, cfg->new_BB_name());
    
    cfg->add_bb(evalRhs_BB);
    cfg->add_bb(after_BB);
    
    string dst = cfg->create_new_tempvar(TYPE_INT);
    string zero = cfg->create_new_tempvar(TYPE_INT);
    
    // Asignar 0 al resultado por defecto (para cuando lhs sea false)
    cfg->current_bb->add_IRInstr(IRInstr::ldconst, TYPE_INT, {zero, "0"});
    cfg->current_bb->add_IRInstr(IRInstr::copy, TYPE_INT, {zero, dst});
    
    // Si lhs != 0, ir a evaluar rhs; si no, ir al final
    cfg->current_bb->test_var_name = lhs;
    cfg->current_bb->exit_true = evalRhs_BB;
    cfg->current_bb->exit_false = after_BB;
    
    // Bloque de evaluación de rhs
    cfg->current_bb = evalRhs_BB;
    string rhs = any_cast<string>(visit(ctx->expr(1)));
    
    // Convertir rhs a booleano (0 o 1)
    string rhs_bool = cfg->create_new_tempvar(TYPE_INT);
    cfg->current_bb->add_IRInstr(IRInstr::cmp_ne, TYPE_INT, {rhs, zero, rhs_bool});
    
    // dst = rhs_bool
    cfg->current_bb->add_IRInstr(IRInstr::copy, TYPE_INT, {rhs_bool, dst});
    cfg->current_bb->exit_true = after_BB;
    
    // Bloque after
    cfg->current_bb = after_BB;
    return dst;
}

antlrcpp::Any IRVisitor::visitOrCond(ifccParser::OrCondContext *ctx)
{
    string lhs = any_cast<string>(visit(ctx->expr(0)));
    
    // Crear bloques
    BasicBlock *evalRhs_BB = new BasicBlock(cfg, cfg->new_BB_name());
    BasicBlock *after_BB = new BasicBlock(cfg, cfg->new_BB_name());
    
    cfg->add_bb(evalRhs_BB);
    cfg->add_bb(after_BB);
    
    string dst = cfg->create_new_tempvar(TYPE_INT);
    string one = cfg->create_new_tempvar(TYPE_INT);
    string zero = cfg->create_new_tempvar(TYPE_INT);
    
    // Asignar 1 al resultado por defecto (para cuando lhs sea true)
    cfg->current_bb->add_IRInstr(IRInstr::ldconst, TYPE_INT, {one, "1"});
    cfg->current_bb->add_IRInstr(IRInstr::copy, TYPE_INT, {one, dst});
    
    // Si lhs == 0, ir a evaluar rhs; si no, ir al final (lhs es true, resultado es 1)
    cfg->current_bb->test_var_name = lhs;
    cfg->current_bb->exit_true = after_BB;
    cfg->current_bb->exit_false = evalRhs_BB;
    
    // Bloque de evaluación de rhs
    cfg->current_bb = evalRhs_BB;
    string rhs = any_cast<string>(visit(ctx->expr(1)));
    
    // Convertir rhs a booleano (0 o 1)
    string rhs_bool = cfg->create_new_tempvar(TYPE_INT);
    cfg->current_bb->add_IRInstr(IRInstr::ldconst, TYPE_INT, {zero, "0"});
    cfg->current_bb->add_IRInstr(IRInstr::cmp_ne, TYPE_INT, {rhs, zero, rhs_bool});
    
    // dst = rhs_bool
    cfg->current_bb->add_IRInstr(IRInstr::copy, TYPE_INT, {rhs_bool, dst});
    cfg->current_bb->exit_true = after_BB;
    
    // Bloque after
    cfg->current_bb = after_BB;
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

antlrcpp::Any IRVisitor::visitAddrOfVar(ifccParser::AddrOfVarContext *ctx)
{
    string name = ctx->VAR()->getText();
    if (!isDeclaredInScope(name))
    {
        cerr << "error: variable '" << name << "' used before declaration in function '"
             << currentFunction << "'\n";
        success = false;
        string zero = cfg->create_new_tempvar(TYPE_PTR);
        cfg->current_bb->add_IRInstr(IRInstr::ldconst, TYPE_INT, {zero, "0"});
        return zero;
    }

    string dst = cfg->create_new_tempvar(TYPE_PTR);
    cfg->current_bb->add_IRInstr(IRInstr::addrof, TYPE_PTR, {dst, name});
    return dst;
}

antlrcpp::Any IRVisitor::visitVarExpr(ifccParser::VarExprContext *ctx)
{
    string name = ctx->VAR()->getText();
    if (!isDeclaredInScope(name))
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

antlrcpp::Any IRVisitor::visitDerefExpr(ifccParser::DerefExprContext *ctx)
{
    string ptr = any_cast<string>(visit(ctx->expr()));
    if (cfg->get_var_type(ptr) != TYPE_PTR)
    {
        cerr << "error: cannot dereference non-pointer in function '" << currentFunction << "'\n";
        success = false;
        string zero = cfg->create_new_tempvar(TYPE_INT);
        cfg->current_bb->add_IRInstr(IRInstr::ldconst, TYPE_INT, {zero, "0"});
        return zero;
    }

    string dst = cfg->create_new_tempvar(TYPE_INT);
    cfg->current_bb->add_IRInstr(IRInstr::rmem, TYPE_INT, {dst, ptr});
    return dst;
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
        int i = 0;
        for (auto e : ctx->arg_list()->expr())
        {
            string arg = any_cast<string>(visit(e));
            params.push_back(arg);
            if (functionParamTypes.count(fname) && i < static_cast<int>(functionParamTypes[fname].size()))
            {
                Type expected = functionParamTypes[fname][i];
                Type got = cfg->get_var_type(arg);
                if (expected != got)
                {
                    cerr << "error: type mismatch for argument " << (i + 1)
                         << " in call to '" << fname << "'\n";
                    success = false;
                }
            }
            i++;
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
    scopeStack.push_back({});
    for (auto s : ctx->stmt())
    {
        visit(s);
    }
    scopeStack.pop_back();
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
