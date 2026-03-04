
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

  virtual std::any visitVar_decl_list(ifccParser::Var_decl_listContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitVar_decl(ifccParser::Var_declContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAssign_stmt(ifccParser::Assign_stmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitReturn_stmt(ifccParser::Return_stmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExpr(ifccParser::ExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitOrExpr(ifccParser::OrExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitXorExpr(ifccParser::XorExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAndExpr(ifccParser::AndExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitEqExpr(ifccParser::EqExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitRelExpr(ifccParser::RelExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAddExpr(ifccParser::AddExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitMulExpr(ifccParser::MulExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitUnaryExpr(ifccParser::UnaryExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPrimary(ifccParser::PrimaryContext *ctx) override {
    return visitChildren(ctx);
  }


};

