#include "IR.h"
#include <iostream>

using namespace std;


/* ================= Fonctions utilitaires ================= */
//Pour les message d'erreur de types de retour 
std::string typeToString(Type t)
{
    switch (t)
    {
        case TYPE_INT:  return "int";
        case TYPE_PTR:  return "pointer";
        case TYPE_VOID: return "void";
        default:        return "unknown";
    }
}


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

            if (t == TYPE_PTR)
            {
                o << "    movq $" << val << ", " << cfg->IR_reg_to_asm(dest) << "\n";
            }
            else
            {
                o << "    movl $" << val << ", " << cfg->IR_reg_to_asm(dest) << "\n";
            }
            break;
        }

        case copy:
        {
            // params: src, dest
            string src  = params[0];
            string dest = params[1];

            if (t == TYPE_PTR)
            {
                o << "    movq " << cfg->IR_reg_to_asm(src) << ", %rax\n";
                o << "    movq %rax, " << cfg->IR_reg_to_asm(dest) << "\n";
            }
            else
            {
                o << "    movl " << cfg->IR_reg_to_asm(src) << ", %ecx\n";
                o << "    movl %ecx, " << cfg->IR_reg_to_asm(dest) << "\n";
            }
            break;
        }

        case addrof:
        {
            // params: dest_ptr, source_var
            string dest = params[0];
            string src  = params[1];
            o << "    leaq " << cfg->IR_reg_to_asm(src) << ", %rax\n";
            o << "    movq %rax, " << cfg->IR_reg_to_asm(dest) << "\n";
            break;
        }

        case rmem:
        {
            // params: dest_int, src_ptr
            string dest = params[0];
            string ptr  = params[1];
            o << "    movq " << cfg->IR_reg_to_asm(ptr) << ", %rax\n";
            o << "    movl (%rax), %ecx\n";
            o << "    movl %ecx, " << cfg->IR_reg_to_asm(dest) << "\n";
            break;
        }

        case wmem:
        {
            // params: dst_ptr, src_int
            string ptr = params[0];
            string src = params[1];
            o << "    movq " << cfg->IR_reg_to_asm(ptr) << ", %rax\n";
            o << "    movl " << cfg->IR_reg_to_asm(src) << ", %ecx\n";
            o << "    movl %ecx, (%rax)\n";
            break;
        }

        case add:
        case sub:
        case mul:
        case div:
        case mod:
        case band:
        case bxor:
        case bor:
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
            else if (op == div)
            {
                o << "    cltd\n";
                o << "    idivl %ecx\n";
            }
            else if (op == mod)
            {
                o << "    cltd\n";
                o << "    idivl %ecx\n";
                o << "    movl %edx, %eax\n";
            }
            else if (op == band)
                o << "    andl %ecx, %eax\n";
            else if (op == bxor)
                o << "    xorl %ecx, %eax\n";
            else if (op == bor)
                o << "    orl %ecx, %eax\n";

            o << "    movl %eax, " << cfg->IR_reg_to_asm(dest) << "\n";
            break;
        }

        case cmp_eq:
        case cmp_ne:
        case cmp_lt:
        case cmp_le:
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
            else if (op == cmp_ne)
                o << "    setne %al\n";
            else if (op == cmp_lt)
                o << "    setl %al\n";
            else if (op == cmp_le)
                o << "    setle %al\n";

            o << "    movzbl %al, %eax\n";
            o << "    movl %eax, " << cfg->IR_reg_to_asm(dest) << "\n";
            break;
        }

        case call:
        {
            // params: function_name, dest, arg1, arg2, ...
            string fname = params[0];
            string dest  = params[1];
            static const char *argRegsInt[] = {
                "%edi", "%esi", "%edx", "%ecx", "%r8d", "%r9d"
            };
            static const char *argRegsPtr[] = {
                "%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"
            };

            int argCount = static_cast<int>(params.size()) - 2;
            for (int i = 0; i < argCount && i < 6; i++)
            {
                Type argType = cfg->get_var_type(params[i + 2]);
                if (argType == TYPE_PTR)
                {
                    o << "    movq " << cfg->IR_reg_to_asm(params[i + 2]) << ", " << argRegsPtr[i] << "\n";
                }
                else
                {
                    o << "    movl " << cfg->IR_reg_to_asm(params[i + 2]) << ", " << argRegsInt[i] << "\n";
                }
            }

            // Avoid leaking a previous return value when the callee returns void.
            o << "    movl $0, %eax\n";
            o << "    call " << fname << "\n";
            o << "    movl %eax, " << cfg->IR_reg_to_asm(dest) << "\n";
            break;
        }

        case ret:
        {
            if (params.empty())
            {
                if (t != TYPE_VOID)
                {
                    o << "    movl $0, %eax\n";
                }
            }
            else
            {
                o << "    movl " << cfg->IR_reg_to_asm(params[0]) << ", %eax\n";
            }
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
        o << "    movl " << cfg->IR_reg_to_asm(test_var_name) << ", %ecx\n";
        o << "    cmpl $0, %ecx\n";
        o << "    je " << exit_false->label << "\n";
        o << "    jmp " << exit_true->label << "\n";
    }
}

/* ================= CFG ================= */

CFG::CFG(const string& functionName_)
{
    functionName = functionName_;
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
    return functionName + "_BB" + to_string(nextBBnumber++);
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
    o << ".globl " << functionName << "\n";
    o << functionName << ":\n";

    o << "    pushq %rbp\n";
    o << "    movq %rsp, %rbp\n";

    int frameSize = -nextFreeSymbolIndex - 4;
    if (frameSize < 0)
    {
        frameSize = 0;
    }
    if (frameSize % 16 != 0)
    {
        frameSize += 16 - (frameSize % 16);
    }
    if (frameSize > 0)
    {
        o << "    subq $" << frameSize << ", %rsp\n";
    }

    static const char *argRegs[] = {
        "%edi", "%esi", "%edx", "%ecx", "%r8d", "%r9d"
    };

    for (size_t i = 0; i < paramOrder.size() && i < 6; i++)
    {
        o << "    movl " << argRegs[i] << ", " << IR_reg_to_asm(paramOrder[i]) << "\n";
    }
}

void CFG::gen_asm_epilogue(ostream& o)
{
    o << "    movq %rbp, %rsp\n";
    o << "    popq %rbp\n";
    o << "    ret\n";
}

string CFG::IR_reg_to_asm(string reg)
{
    int idx = SymbolIndex.at(reg);
    return to_string(idx) + "(%rbp)";
}

void CFG::add_to_symbol_table(string name, Type t)
{
    SymbolType[name] = t;
    SymbolIndex[name] = nextFreeSymbolIndex;
    if (t == TYPE_PTR)
    {
        nextFreeSymbolIndex -= 8;
    }
    else
    {
        nextFreeSymbolIndex -= 4;
    }
}

void CFG::set_param_order(const vector<string>& params)
{
    paramOrder = params;
}

bool CFG::has_symbol(const string& name) const
{
    return SymbolIndex.count(name) != 0;
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



