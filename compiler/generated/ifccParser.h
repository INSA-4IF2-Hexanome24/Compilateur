
// Generated from ifcc.g4 by ANTLR 4.13.2

#pragma once


#include "antlr4-runtime.h"




class  ifccParser : public antlr4::Parser {
public:
  enum {
    INT = 1, RETURN = 2, MAIN = 3, EQ = 4, NEQ = 5, ASSIGN = 6, LT = 7, 
    GT = 8, PLUS = 9, MINUS = 10, STAR = 11, SLASH = 12, PERCENT = 13, BAND = 14, 
    BXOR = 15, BOR = 16, NOT = 17, LPAREN = 18, RPAREN = 19, LBRACE = 20, 
    RBRACE = 21, SEMI = 22, COMMA = 23, CONST = 24, VAR = 25, COMMENT = 26, 
    DIRECTIVE = 27, WS = 28
  };

  enum {
    RuleAxiom = 0, RuleProg = 1, RuleStmt = 2, RuleDecl_stmt = 3, RuleVar_decl_list = 4, 
    RuleVar_decl = 5, RuleAssign_stmt = 6, RuleReturn_stmt = 7, RuleExpr = 8, 
    RuleOrExpr = 9, RuleXorExpr = 10, RuleAndExpr = 11, RuleEqExpr = 12, 
    RuleRelExpr = 13, RuleAddExpr = 14, RuleMulExpr = 15, RuleUnaryExpr = 16, 
    RulePrimary = 17
  };

  explicit ifccParser(antlr4::TokenStream *input);

  ifccParser(antlr4::TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options);

  ~ifccParser() override;

  std::string getGrammarFileName() const override;

  const antlr4::atn::ATN& getATN() const override;

  const std::vector<std::string>& getRuleNames() const override;

  const antlr4::dfa::Vocabulary& getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;


  class AxiomContext;
  class ProgContext;
  class StmtContext;
  class Decl_stmtContext;
  class Var_decl_listContext;
  class Var_declContext;
  class Assign_stmtContext;
  class Return_stmtContext;
  class ExprContext;
  class OrExprContext;
  class XorExprContext;
  class AndExprContext;
  class EqExprContext;
  class RelExprContext;
  class AddExprContext;
  class MulExprContext;
  class UnaryExprContext;
  class PrimaryContext; 

  class  AxiomContext : public antlr4::ParserRuleContext {
  public:
    AxiomContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ProgContext *prog();
    antlr4::tree::TerminalNode *EOF();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AxiomContext* axiom();

  class  ProgContext : public antlr4::ParserRuleContext {
  public:
    ProgContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *INT();
    antlr4::tree::TerminalNode *MAIN();
    antlr4::tree::TerminalNode *LPAREN();
    antlr4::tree::TerminalNode *RPAREN();
    antlr4::tree::TerminalNode *LBRACE();
    Return_stmtContext *return_stmt();
    antlr4::tree::TerminalNode *RBRACE();
    std::vector<StmtContext *> stmt();
    StmtContext* stmt(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ProgContext* prog();

  class  StmtContext : public antlr4::ParserRuleContext {
  public:
    StmtContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Decl_stmtContext *decl_stmt();
    Assign_stmtContext *assign_stmt();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  StmtContext* stmt();

  class  Decl_stmtContext : public antlr4::ParserRuleContext {
  public:
    Decl_stmtContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *INT();
    Var_decl_listContext *var_decl_list();
    antlr4::tree::TerminalNode *SEMI();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Decl_stmtContext* decl_stmt();

  class  Var_decl_listContext : public antlr4::ParserRuleContext {
  public:
    Var_decl_listContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Var_declContext *> var_decl();
    Var_declContext* var_decl(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Var_decl_listContext* var_decl_list();

  class  Var_declContext : public antlr4::ParserRuleContext {
  public:
    Var_declContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *VAR();
    antlr4::tree::TerminalNode *ASSIGN();
    ExprContext *expr();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Var_declContext* var_decl();

  class  Assign_stmtContext : public antlr4::ParserRuleContext {
  public:
    Assign_stmtContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *VAR();
    antlr4::tree::TerminalNode *ASSIGN();
    ExprContext *expr();
    antlr4::tree::TerminalNode *SEMI();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Assign_stmtContext* assign_stmt();

  class  Return_stmtContext : public antlr4::ParserRuleContext {
  public:
    Return_stmtContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *RETURN();
    ExprContext *expr();
    antlr4::tree::TerminalNode *SEMI();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Return_stmtContext* return_stmt();

  class  ExprContext : public antlr4::ParserRuleContext {
  public:
    ExprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    OrExprContext *orExpr();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ExprContext* expr();

  class  OrExprContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *op = nullptr;
    OrExprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<XorExprContext *> xorExpr();
    XorExprContext* xorExpr(size_t i);
    std::vector<antlr4::tree::TerminalNode *> BOR();
    antlr4::tree::TerminalNode* BOR(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  OrExprContext* orExpr();

  class  XorExprContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *op = nullptr;
    XorExprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<AndExprContext *> andExpr();
    AndExprContext* andExpr(size_t i);
    std::vector<antlr4::tree::TerminalNode *> BXOR();
    antlr4::tree::TerminalNode* BXOR(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  XorExprContext* xorExpr();

  class  AndExprContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *op = nullptr;
    AndExprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<EqExprContext *> eqExpr();
    EqExprContext* eqExpr(size_t i);
    std::vector<antlr4::tree::TerminalNode *> BAND();
    antlr4::tree::TerminalNode* BAND(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AndExprContext* andExpr();

  class  EqExprContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *op = nullptr;
    EqExprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<RelExprContext *> relExpr();
    RelExprContext* relExpr(size_t i);
    std::vector<antlr4::tree::TerminalNode *> EQ();
    antlr4::tree::TerminalNode* EQ(size_t i);
    std::vector<antlr4::tree::TerminalNode *> NEQ();
    antlr4::tree::TerminalNode* NEQ(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  EqExprContext* eqExpr();

  class  RelExprContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *op = nullptr;
    RelExprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<AddExprContext *> addExpr();
    AddExprContext* addExpr(size_t i);
    antlr4::tree::TerminalNode *LT();
    antlr4::tree::TerminalNode *GT();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  RelExprContext* relExpr();

  class  AddExprContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *op = nullptr;
    AddExprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<MulExprContext *> mulExpr();
    MulExprContext* mulExpr(size_t i);
    std::vector<antlr4::tree::TerminalNode *> PLUS();
    antlr4::tree::TerminalNode* PLUS(size_t i);
    std::vector<antlr4::tree::TerminalNode *> MINUS();
    antlr4::tree::TerminalNode* MINUS(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AddExprContext* addExpr();

  class  MulExprContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *op = nullptr;
    MulExprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<UnaryExprContext *> unaryExpr();
    UnaryExprContext* unaryExpr(size_t i);
    std::vector<antlr4::tree::TerminalNode *> STAR();
    antlr4::tree::TerminalNode* STAR(size_t i);
    std::vector<antlr4::tree::TerminalNode *> SLASH();
    antlr4::tree::TerminalNode* SLASH(size_t i);
    std::vector<antlr4::tree::TerminalNode *> PERCENT();
    antlr4::tree::TerminalNode* PERCENT(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  MulExprContext* mulExpr();

  class  UnaryExprContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *op = nullptr;
    UnaryExprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    UnaryExprContext *unaryExpr();
    antlr4::tree::TerminalNode *NOT();
    antlr4::tree::TerminalNode *MINUS();
    PrimaryContext *primary();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  UnaryExprContext* unaryExpr();

  class  PrimaryContext : public antlr4::ParserRuleContext {
  public:
    PrimaryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *CONST();
    antlr4::tree::TerminalNode *VAR();
    antlr4::tree::TerminalNode *LPAREN();
    ExprContext *expr();
    antlr4::tree::TerminalNode *RPAREN();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PrimaryContext* primary();


  // By default the static state used to implement the parser is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:
};

