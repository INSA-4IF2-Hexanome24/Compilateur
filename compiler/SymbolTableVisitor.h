#pragma once

#include <iostream>
#include <map>
#include <set>
#include <string>

#include "antlr4-runtime.h"
#include "generated/ifccBaseVisitor.h"

class SymbolTableVisitor : public ifccBaseVisitor {
public:
    // Maps variable name -> stack index (negative offset from %rbp, multiple of 4)
    std::map<std::string, int> symbolTable;

    // Returns false if any semantic error was found
    bool success = true;

    antlrcpp::Any visitProg(ifccParser::ProgContext *ctx) override;
    antlrcpp::Any visitDecl_stmt(ifccParser::Decl_stmtContext *ctx) override;
    antlrcpp::Any visitAssign_stmt(ifccParser::Assign_stmtContext *ctx) override;
    antlrcpp::Any visitReturn_stmt(ifccParser::Return_stmtContext *ctx) override;

    antlrcpp::Any visitPrimary(ifccParser::PrimaryContext *ctx) override;

private:
    int nextIndex = -4; // next available stack slot
    std::set<std::string> used; // variables that have been read
};