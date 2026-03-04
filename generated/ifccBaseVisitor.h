
// Generated from ifcc.g4 by ANTLR 4.13.2

#pragma once


#include "antlr4-runtime.h"
#include "ifccVisitor.h"


/**
 * This class provides an empty implementation of ifccVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  ifccBaseVisitor : public ifccVisitor {
public:

  virtual std::any visitAxiom(ifccParser::AxiomContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitProg(ifccParser::ProgContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitStmt(ifccParser::StmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitDecl_stmt(ifccParser::Decl_stmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAssign_stmt(ifccParser::Assign_stmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitReturn_stmt(ifccParser::Return_stmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitNotExpr(ifccParser::NotExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitNegExpr(ifccParser::NegExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitParenExpr(ifccParser::ParenExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitConstExpr(ifccParser::ConstExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitVarExpr(ifccParser::VarExprContext *ctx) override {
    return visitChildren(ctx);
  }


};

