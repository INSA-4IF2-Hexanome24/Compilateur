#include "IR.h"
#include <iostream>

using namespace std;

/* ================= IRInstr ================= */

IRInstr::IRInstr(BasicBlock* bb_, Operation op_, Type t_, vector<string> params_)
{
    bb = bb_;
    op = op_;
    t = t_;
    params = params_;
}

void IRInstr::gen_asm(ostream &o)
{
    CFG* cfg = bb->cfg;

    switch(op)
    {
        case ldconst:
        {
            // params: dest, constant
            string dest = params[0];
            string val  = params[1];

            o << "    movl $" << val << ", " << cfg->IR_reg_to_asm(dest) << "\n";
            break;
        }

        case copy:
        {
            // params: src, dest
            string src  = params[0];
            string dest = params[1];

            o << "    movl " << cfg->IR_reg_to_asm(src) << ", %eax\n";
            o << "    movl %eax, " << cfg->IR_reg_to_asm(dest) << "\n";
            break;
        }

        case add:
        case sub:
        case mul:
        {
            // params: x, y, dest
            string x = params[0];
            string y = params[1];
            string dest = params[2];

            o << "    movl " << cfg->IR_reg_to_asm(x) << ", %eax\n";
            o << "    movl " << cfg->IR_reg_to_asm(y) << ", %ecx\n";

            if (op == add)
                o << "    addl %ecx, %eax\n";
            else if (op == sub)
                o << "    subl %ecx, %eax\n";
            else if (op == mul)
                o << "    imull %ecx, %eax\n";

            o << "    movl %eax, " << cfg->IR_reg_to_asm(dest) << "\n";
            break;
        }

        case cmp_eq:
        case cmp_lt:
        case cmp_gt:
        {
            // params: x, y, dest
            string x = params[0];
            string y = params[1];
            string dest = params[2];

            o << "    movl " << cfg->IR_reg_to_asm(x) << ", %eax\n";
            o << "    movl " << cfg->IR_reg_to_asm(y) << ", %ecx\n";
            o << "    cmpl %ecx, %eax\n";

            if (op == cmp_eq)
                o << "    sete %al\n";
            else if (op == cmp_lt)
                o << "    setl %al\n";
            else if (op == cmp_gt)
                o << "    setg %al\n";

            o << "    movzbl %al, %eax\n";
            o << "    movl %eax, " << cfg->IR_reg_to_asm(dest) << "\n";
            break;
        }

        case call:
        {
            // params: function_name, dest
            string fname = params[0];
            string dest  = params[1];

            o << "    call " << fname << "\n";
            o << "    movl %eax, " << cfg->IR_reg_to_asm(dest) << "\n";
            break;
        }

        case ret:
        {
            string retval = params[0];
            o << "    movl " << cfg->IR_reg_to_asm(retval) << ", %eax\n";
            o << "    jmp " << cfg->bbs.back()->label << "\n"; 
            break;
        }

        

        default:
            cerr << "Unsupported IR operation\n";
    }
}

/* ================= BasicBlock ================= */

BasicBlock::BasicBlock(CFG* cfg_, string entry_label)
{
    cfg = cfg_;
    label = entry_label;
    exit_true = nullptr;
    exit_false = nullptr;
}

void BasicBlock::add_IRInstr(IRInstr::Operation op, Type t, vector<string> params)
{
    instrs.push_back(new IRInstr(this, op, t, params));
}

void BasicBlock::gen_asm(ostream &o)
{
    o << label << ":\n";

    for (auto instr : instrs)
    {
        instr->gen_asm(o);
    }

    if (exit_true == nullptr)
    {
        cfg->gen_asm_epilogue(o);
    }
    else if (exit_false == nullptr)
    {
        o << "    jmp " << exit_true->label << "\n";
    }
    else
    {
        o << "    movl " << cfg->IR_reg_to_asm(test_var_name) << ", %eax\n";
        o << "    cmpl $0, %eax\n";
        o << "    je " << exit_false->label << "\n";
        o << "    jmp " << exit_true->label << "\n";
    }
}

/* ================= CFG ================= */

CFG::CFG(DefFonction* ast_)
{
    ast = ast_;
    nextFreeSymbolIndex = -4;
    nextBBnumber = 0;
    current_bb = nullptr;
}

void CFG::add_bb(BasicBlock* bb)
{
    bbs.push_back(bb);
}

string CFG::new_BB_name()
{
    return "BB" + to_string(nextBBnumber++);
}

void CFG::gen_asm(ostream& o)
{
    gen_asm_prologue(o);

    for (auto bb : bbs)
    {
        bb->gen_asm(o);
    }
}

void CFG::gen_asm_prologue(ostream& o)
{
#ifdef __APPLE__
    o << ".globl _main\n_main:\n";
#else
    o << ".globl main\nmain:\n";
#endif

    o << "    pushq %rbp\n";
    o << "    movq %rsp, %rbp\n";
}

void CFG::gen_asm_epilogue(ostream& o)
{
    o << "    movq %rbp, %rsp\n";
    o << "    popq %rbp\n";
    o << "    ret\n";
}

string CFG::IR_reg_to_asm(string reg)
{
    int idx = SymbolIndex[reg];
    return to_string(idx) + "(%rbp)";
}

void CFG::add_to_symbol_table(string name, Type t)
{
    SymbolType[name] = t;
    SymbolIndex[name] = nextFreeSymbolIndex;
    nextFreeSymbolIndex -= 4;
}

string CFG::create_new_tempvar(Type t)
{
    string name = "_tmp" + to_string(SymbolIndex.size());
    add_to_symbol_table(name, t);
    return name;
}

int CFG::get_var_index(string name)
{
    return SymbolIndex[name];
}

Type CFG::get_var_type(string name)
{
    return SymbolType[name];
}