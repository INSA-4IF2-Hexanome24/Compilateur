
// Generated from ifcc.g4 by ANTLR 4.13.2

#pragma once


#include "antlr4-runtime.h"
#include "ifccParser.h"



/**
 * This class defines an abstract visitor for a parse tree
 * produced by ifccParser.
 */
class  ifccVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by ifccParser.
   */
    virtual std::any visitAxiom(ifccParser::AxiomContext *context) = 0;

    virtual std::any visitProg(ifccParser::ProgContext *context) = 0;

    virtual std::any visitStmt(ifccParser::StmtContext *context) = 0;

    virtual std::any visitDecl_stmt(ifccParser::Decl_stmtContext *context) = 0;

    virtual std::any visitVar_decl_list(ifccParser::Var_decl_listContext *context) = 0;

    virtual std::any visitVar_decl(ifccParser::Var_declContext *context) = 0;

    virtual std::any visitAssign_stmt(ifccParser::Assign_stmtContext *context) = 0;

    virtual std::any visitReturn_stmt(ifccParser::Return_stmtContext *context) = 0;

    virtual std::any visitExpr(ifccParser::ExprContext *context) = 0;

    virtual std::any visitOrExpr(ifccParser::OrExprContext *context) = 0;

    virtual std::any visitXorExpr(ifccParser::XorExprContext *context) = 0;

    virtual std::any visitAndExpr(ifccParser::AndExprContext *context) = 0;

    virtual std::any visitEqExpr(ifccParser::EqExprContext *context) = 0;

    virtual std::any visitRelExpr(ifccParser::RelExprContext *context) = 0;

    virtual std::any visitAddExpr(ifccParser::AddExprContext *context) = 0;

    virtual std::any visitMulExpr(ifccParser::MulExprContext *context) = 0;

    virtual std::any visitUnaryExpr(ifccParser::UnaryExprContext *context) = 0;

    virtual std::any visitPrimary(ifccParser::PrimaryContext *context) = 0;


};

