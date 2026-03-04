
// Generated from ifcc.g4 by ANTLR 4.13.2


#include "ifccVisitor.h"

#include "ifccParser.h"


using namespace antlrcpp;

using namespace antlr4;

namespace {

struct IfccParserStaticData final {
  IfccParserStaticData(std::vector<std::string> ruleNames,
                        std::vector<std::string> literalNames,
                        std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  IfccParserStaticData(const IfccParserStaticData&) = delete;
  IfccParserStaticData(IfccParserStaticData&&) = delete;
  IfccParserStaticData& operator=(const IfccParserStaticData&) = delete;
  IfccParserStaticData& operator=(IfccParserStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

::antlr4::internal::OnceFlag ifccParserOnceFlag;
#if ANTLR4_USE_THREAD_LOCAL_CACHE
static thread_local
#endif
std::unique_ptr<IfccParserStaticData> ifccParserStaticData = nullptr;

void ifccParserInitialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  if (ifccParserStaticData != nullptr) {
    return;
  }
#else
  assert(ifccParserStaticData == nullptr);
#endif
  auto staticData = std::make_unique<IfccParserStaticData>(
    std::vector<std::string>{
      "axiom", "prog", "stmt", "decl_stmt", "var_decl_list", "var_decl", 
      "assign_stmt", "return_stmt", "expr", "orExpr", "xorExpr", "andExpr", 
      "eqExpr", "relExpr", "addExpr", "mulExpr", "unaryExpr", "primary"
    },
    std::vector<std::string>{
      "", "'int'", "'return'", "'main'", "'=='", "'!='", "'='", "'<'", "'>'", 
      "'+'", "'-'", "'*'", "'/'", "'%'", "'&'", "'^'", "'|'", "'!'", "'('", 
      "')'", "'{'", "'}'", "';'", "','"
    },
    std::vector<std::string>{
      "", "INT", "RETURN", "MAIN", "EQ", "NEQ", "ASSIGN", "LT", "GT", "PLUS", 
      "MINUS", "STAR", "SLASH", "PERCENT", "BAND", "BXOR", "BOR", "NOT", 
      "LPAREN", "RPAREN", "LBRACE", "RBRACE", "SEMI", "COMMA", "CONST", 
      "VAR", "COMMENT", "DIRECTIVE", "WS"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,1,28,152,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,6,2,
  	7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,2,14,7,
  	14,2,15,7,15,2,16,7,16,2,17,7,17,1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,
  	5,1,46,8,1,10,1,12,1,49,9,1,1,1,1,1,1,1,1,2,1,2,3,2,56,8,2,1,3,1,3,1,
  	3,1,3,1,4,1,4,1,4,5,4,65,8,4,10,4,12,4,68,9,4,1,5,1,5,1,5,3,5,73,8,5,
  	1,6,1,6,1,6,1,6,1,6,1,7,1,7,1,7,1,7,1,8,1,8,1,9,1,9,1,9,5,9,89,8,9,10,
  	9,12,9,92,9,9,1,10,1,10,1,10,5,10,97,8,10,10,10,12,10,100,9,10,1,11,1,
  	11,1,11,5,11,105,8,11,10,11,12,11,108,9,11,1,12,1,12,1,12,5,12,113,8,
  	12,10,12,12,12,116,9,12,1,13,1,13,1,13,3,13,121,8,13,1,14,1,14,1,14,5,
  	14,126,8,14,10,14,12,14,129,9,14,1,15,1,15,1,15,5,15,134,8,15,10,15,12,
  	15,137,9,15,1,16,1,16,1,16,3,16,142,8,16,1,17,1,17,1,17,1,17,1,17,1,17,
  	3,17,150,8,17,1,17,0,0,18,0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,
  	32,34,0,5,1,0,4,5,1,0,7,8,1,0,9,10,1,0,11,13,2,0,10,10,17,17,147,0,36,
  	1,0,0,0,2,39,1,0,0,0,4,55,1,0,0,0,6,57,1,0,0,0,8,61,1,0,0,0,10,69,1,0,
  	0,0,12,74,1,0,0,0,14,79,1,0,0,0,16,83,1,0,0,0,18,85,1,0,0,0,20,93,1,0,
  	0,0,22,101,1,0,0,0,24,109,1,0,0,0,26,117,1,0,0,0,28,122,1,0,0,0,30,130,
  	1,0,0,0,32,141,1,0,0,0,34,149,1,0,0,0,36,37,3,2,1,0,37,38,5,0,0,1,38,
  	1,1,0,0,0,39,40,5,1,0,0,40,41,5,3,0,0,41,42,5,18,0,0,42,43,5,19,0,0,43,
  	47,5,20,0,0,44,46,3,4,2,0,45,44,1,0,0,0,46,49,1,0,0,0,47,45,1,0,0,0,47,
  	48,1,0,0,0,48,50,1,0,0,0,49,47,1,0,0,0,50,51,3,14,7,0,51,52,5,21,0,0,
  	52,3,1,0,0,0,53,56,3,6,3,0,54,56,3,12,6,0,55,53,1,0,0,0,55,54,1,0,0,0,
  	56,5,1,0,0,0,57,58,5,1,0,0,58,59,3,8,4,0,59,60,5,22,0,0,60,7,1,0,0,0,
  	61,66,3,10,5,0,62,63,5,23,0,0,63,65,3,10,5,0,64,62,1,0,0,0,65,68,1,0,
  	0,0,66,64,1,0,0,0,66,67,1,0,0,0,67,9,1,0,0,0,68,66,1,0,0,0,69,72,5,25,
  	0,0,70,71,5,6,0,0,71,73,3,16,8,0,72,70,1,0,0,0,72,73,1,0,0,0,73,11,1,
  	0,0,0,74,75,5,25,0,0,75,76,5,6,0,0,76,77,3,16,8,0,77,78,5,22,0,0,78,13,
  	1,0,0,0,79,80,5,2,0,0,80,81,3,16,8,0,81,82,5,22,0,0,82,15,1,0,0,0,83,
  	84,3,18,9,0,84,17,1,0,0,0,85,90,3,20,10,0,86,87,5,16,0,0,87,89,3,20,10,
  	0,88,86,1,0,0,0,89,92,1,0,0,0,90,88,1,0,0,0,90,91,1,0,0,0,91,19,1,0,0,
  	0,92,90,1,0,0,0,93,98,3,22,11,0,94,95,5,15,0,0,95,97,3,22,11,0,96,94,
  	1,0,0,0,97,100,1,0,0,0,98,96,1,0,0,0,98,99,1,0,0,0,99,21,1,0,0,0,100,
  	98,1,0,0,0,101,106,3,24,12,0,102,103,5,14,0,0,103,105,3,24,12,0,104,102,
  	1,0,0,0,105,108,1,0,0,0,106,104,1,0,0,0,106,107,1,0,0,0,107,23,1,0,0,
  	0,108,106,1,0,0,0,109,114,3,26,13,0,110,111,7,0,0,0,111,113,3,26,13,0,
  	112,110,1,0,0,0,113,116,1,0,0,0,114,112,1,0,0,0,114,115,1,0,0,0,115,25,
  	1,0,0,0,116,114,1,0,0,0,117,120,3,28,14,0,118,119,7,1,0,0,119,121,3,28,
  	14,0,120,118,1,0,0,0,120,121,1,0,0,0,121,27,1,0,0,0,122,127,3,30,15,0,
  	123,124,7,2,0,0,124,126,3,30,15,0,125,123,1,0,0,0,126,129,1,0,0,0,127,
  	125,1,0,0,0,127,128,1,0,0,0,128,29,1,0,0,0,129,127,1,0,0,0,130,135,3,
  	32,16,0,131,132,7,3,0,0,132,134,3,32,16,0,133,131,1,0,0,0,134,137,1,0,
  	0,0,135,133,1,0,0,0,135,136,1,0,0,0,136,31,1,0,0,0,137,135,1,0,0,0,138,
  	139,7,4,0,0,139,142,3,32,16,0,140,142,3,34,17,0,141,138,1,0,0,0,141,140,
  	1,0,0,0,142,33,1,0,0,0,143,150,5,24,0,0,144,150,5,25,0,0,145,146,5,18,
  	0,0,146,147,3,16,8,0,147,148,5,19,0,0,148,150,1,0,0,0,149,143,1,0,0,0,
  	149,144,1,0,0,0,149,145,1,0,0,0,150,35,1,0,0,0,13,47,55,66,72,90,98,106,
  	114,120,127,135,141,149
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  ifccParserStaticData = std::move(staticData);
}

}

ifccParser::ifccParser(TokenStream *input) : ifccParser(input, antlr4::atn::ParserATNSimulatorOptions()) {}

ifccParser::ifccParser(TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options) : Parser(input) {
  ifccParser::initialize();
  _interpreter = new atn::ParserATNSimulator(this, *ifccParserStaticData->atn, ifccParserStaticData->decisionToDFA, ifccParserStaticData->sharedContextCache, options);
}

ifccParser::~ifccParser() {
  delete _interpreter;
}

const atn::ATN& ifccParser::getATN() const {
  return *ifccParserStaticData->atn;
}

std::string ifccParser::getGrammarFileName() const {
  return "ifcc.g4";
}

const std::vector<std::string>& ifccParser::getRuleNames() const {
  return ifccParserStaticData->ruleNames;
}

const dfa::Vocabulary& ifccParser::getVocabulary() const {
  return ifccParserStaticData->vocabulary;
}

antlr4::atn::SerializedATNView ifccParser::getSerializedATN() const {
  return ifccParserStaticData->serializedATN;
}


//----------------- AxiomContext ------------------------------------------------------------------

ifccParser::AxiomContext::AxiomContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ifccParser::ProgContext* ifccParser::AxiomContext::prog() {
  return getRuleContext<ifccParser::ProgContext>(0);
}

tree::TerminalNode* ifccParser::AxiomContext::EOF() {
  return getToken(ifccParser::EOF, 0);
}


size_t ifccParser::AxiomContext::getRuleIndex() const {
  return ifccParser::RuleAxiom;
}


std::any ifccParser::AxiomContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ifccVisitor*>(visitor))
    return parserVisitor->visitAxiom(this);
  else
    return visitor->visitChildren(this);
}

ifccParser::AxiomContext* ifccParser::axiom() {
  AxiomContext *_localctx = _tracker.createInstance<AxiomContext>(_ctx, getState());
  enterRule(_localctx, 0, ifccParser::RuleAxiom);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(36);
    prog();
    setState(37);
    match(ifccParser::EOF);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ProgContext ------------------------------------------------------------------

ifccParser::ProgContext::ProgContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ifccParser::ProgContext::INT() {
  return getToken(ifccParser::INT, 0);
}

tree::TerminalNode* ifccParser::ProgContext::MAIN() {
  return getToken(ifccParser::MAIN, 0);
}

tree::TerminalNode* ifccParser::ProgContext::LPAREN() {
  return getToken(ifccParser::LPAREN, 0);
}

tree::TerminalNode* ifccParser::ProgContext::RPAREN() {
  return getToken(ifccParser::RPAREN, 0);
}

tree::TerminalNode* ifccParser::ProgContext::LBRACE() {
  return getToken(ifccParser::LBRACE, 0);
}

ifccParser::Return_stmtContext* ifccParser::ProgContext::return_stmt() {
  return getRuleContext<ifccParser::Return_stmtContext>(0);
}

tree::TerminalNode* ifccParser::ProgContext::RBRACE() {
  return getToken(ifccParser::RBRACE, 0);
}

std::vector<ifccParser::StmtContext *> ifccParser::ProgContext::stmt() {
  return getRuleContexts<ifccParser::StmtContext>();
}

ifccParser::StmtContext* ifccParser::ProgContext::stmt(size_t i) {
  return getRuleContext<ifccParser::StmtContext>(i);
}


size_t ifccParser::ProgContext::getRuleIndex() const {
  return ifccParser::RuleProg;
}


std::any ifccParser::ProgContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ifccVisitor*>(visitor))
    return parserVisitor->visitProg(this);
  else
    return visitor->visitChildren(this);
}

ifccParser::ProgContext* ifccParser::prog() {
  ProgContext *_localctx = _tracker.createInstance<ProgContext>(_ctx, getState());
  enterRule(_localctx, 2, ifccParser::RuleProg);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(39);
    match(ifccParser::INT);
    setState(40);
    match(ifccParser::MAIN);
    setState(41);
    match(ifccParser::LPAREN);
    setState(42);
    match(ifccParser::RPAREN);
    setState(43);
    match(ifccParser::LBRACE);
    setState(47);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ifccParser::INT

    || _la == ifccParser::VAR) {
      setState(44);
      stmt();
      setState(49);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(50);
    return_stmt();
    setState(51);
    match(ifccParser::RBRACE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StmtContext ------------------------------------------------------------------

ifccParser::StmtContext::StmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ifccParser::Decl_stmtContext* ifccParser::StmtContext::decl_stmt() {
  return getRuleContext<ifccParser::Decl_stmtContext>(0);
}

ifccParser::Assign_stmtContext* ifccParser::StmtContext::assign_stmt() {
  return getRuleContext<ifccParser::Assign_stmtContext>(0);
}


size_t ifccParser::StmtContext::getRuleIndex() const {
  return ifccParser::RuleStmt;
}


std::any ifccParser::StmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ifccVisitor*>(visitor))
    return parserVisitor->visitStmt(this);
  else
    return visitor->visitChildren(this);
}

ifccParser::StmtContext* ifccParser::stmt() {
  StmtContext *_localctx = _tracker.createInstance<StmtContext>(_ctx, getState());
  enterRule(_localctx, 4, ifccParser::RuleStmt);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(55);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ifccParser::INT: {
        enterOuterAlt(_localctx, 1);
        setState(53);
        decl_stmt();
        break;
      }

      case ifccParser::VAR: {
        enterOuterAlt(_localctx, 2);
        setState(54);
        assign_stmt();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Decl_stmtContext ------------------------------------------------------------------

ifccParser::Decl_stmtContext::Decl_stmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ifccParser::Decl_stmtContext::INT() {
  return getToken(ifccParser::INT, 0);
}

ifccParser::Var_decl_listContext* ifccParser::Decl_stmtContext::var_decl_list() {
  return getRuleContext<ifccParser::Var_decl_listContext>(0);
}

tree::TerminalNode* ifccParser::Decl_stmtContext::SEMI() {
  return getToken(ifccParser::SEMI, 0);
}


size_t ifccParser::Decl_stmtContext::getRuleIndex() const {
  return ifccParser::RuleDecl_stmt;
}


std::any ifccParser::Decl_stmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ifccVisitor*>(visitor))
    return parserVisitor->visitDecl_stmt(this);
  else
    return visitor->visitChildren(this);
}

ifccParser::Decl_stmtContext* ifccParser::decl_stmt() {
  Decl_stmtContext *_localctx = _tracker.createInstance<Decl_stmtContext>(_ctx, getState());
  enterRule(_localctx, 6, ifccParser::RuleDecl_stmt);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(57);
    match(ifccParser::INT);
    setState(58);
    var_decl_list();
    setState(59);
    match(ifccParser::SEMI);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Var_decl_listContext ------------------------------------------------------------------

ifccParser::Var_decl_listContext::Var_decl_listContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ifccParser::Var_declContext *> ifccParser::Var_decl_listContext::var_decl() {
  return getRuleContexts<ifccParser::Var_declContext>();
}

ifccParser::Var_declContext* ifccParser::Var_decl_listContext::var_decl(size_t i) {
  return getRuleContext<ifccParser::Var_declContext>(i);
}

std::vector<tree::TerminalNode *> ifccParser::Var_decl_listContext::COMMA() {
  return getTokens(ifccParser::COMMA);
}

tree::TerminalNode* ifccParser::Var_decl_listContext::COMMA(size_t i) {
  return getToken(ifccParser::COMMA, i);
}


size_t ifccParser::Var_decl_listContext::getRuleIndex() const {
  return ifccParser::RuleVar_decl_list;
}


std::any ifccParser::Var_decl_listContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ifccVisitor*>(visitor))
    return parserVisitor->visitVar_decl_list(this);
  else
    return visitor->visitChildren(this);
}

ifccParser::Var_decl_listContext* ifccParser::var_decl_list() {
  Var_decl_listContext *_localctx = _tracker.createInstance<Var_decl_listContext>(_ctx, getState());
  enterRule(_localctx, 8, ifccParser::RuleVar_decl_list);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(61);
    var_decl();
    setState(66);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ifccParser::COMMA) {
      setState(62);
      match(ifccParser::COMMA);
      setState(63);
      var_decl();
      setState(68);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Var_declContext ------------------------------------------------------------------

ifccParser::Var_declContext::Var_declContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ifccParser::Var_declContext::VAR() {
  return getToken(ifccParser::VAR, 0);
}

tree::TerminalNode* ifccParser::Var_declContext::ASSIGN() {
  return getToken(ifccParser::ASSIGN, 0);
}

ifccParser::ExprContext* ifccParser::Var_declContext::expr() {
  return getRuleContext<ifccParser::ExprContext>(0);
}


size_t ifccParser::Var_declContext::getRuleIndex() const {
  return ifccParser::RuleVar_decl;
}


std::any ifccParser::Var_declContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ifccVisitor*>(visitor))
    return parserVisitor->visitVar_decl(this);
  else
    return visitor->visitChildren(this);
}

ifccParser::Var_declContext* ifccParser::var_decl() {
  Var_declContext *_localctx = _tracker.createInstance<Var_declContext>(_ctx, getState());
  enterRule(_localctx, 10, ifccParser::RuleVar_decl);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(69);
    match(ifccParser::VAR);
    setState(72);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ifccParser::ASSIGN) {
      setState(70);
      match(ifccParser::ASSIGN);
      setState(71);
      expr();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Assign_stmtContext ------------------------------------------------------------------

ifccParser::Assign_stmtContext::Assign_stmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ifccParser::Assign_stmtContext::VAR() {
  return getToken(ifccParser::VAR, 0);
}

tree::TerminalNode* ifccParser::Assign_stmtContext::ASSIGN() {
  return getToken(ifccParser::ASSIGN, 0);
}

ifccParser::ExprContext* ifccParser::Assign_stmtContext::expr() {
  return getRuleContext<ifccParser::ExprContext>(0);
}

tree::TerminalNode* ifccParser::Assign_stmtContext::SEMI() {
  return getToken(ifccParser::SEMI, 0);
}


size_t ifccParser::Assign_stmtContext::getRuleIndex() const {
  return ifccParser::RuleAssign_stmt;
}


std::any ifccParser::Assign_stmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ifccVisitor*>(visitor))
    return parserVisitor->visitAssign_stmt(this);
  else
    return visitor->visitChildren(this);
}

ifccParser::Assign_stmtContext* ifccParser::assign_stmt() {
  Assign_stmtContext *_localctx = _tracker.createInstance<Assign_stmtContext>(_ctx, getState());
  enterRule(_localctx, 12, ifccParser::RuleAssign_stmt);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(74);
    match(ifccParser::VAR);
    setState(75);
    match(ifccParser::ASSIGN);
    setState(76);
    expr();
    setState(77);
    match(ifccParser::SEMI);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Return_stmtContext ------------------------------------------------------------------

ifccParser::Return_stmtContext::Return_stmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ifccParser::Return_stmtContext::RETURN() {
  return getToken(ifccParser::RETURN, 0);
}

ifccParser::ExprContext* ifccParser::Return_stmtContext::expr() {
  return getRuleContext<ifccParser::ExprContext>(0);
}

tree::TerminalNode* ifccParser::Return_stmtContext::SEMI() {
  return getToken(ifccParser::SEMI, 0);
}


size_t ifccParser::Return_stmtContext::getRuleIndex() const {
  return ifccParser::RuleReturn_stmt;
}


std::any ifccParser::Return_stmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ifccVisitor*>(visitor))
    return parserVisitor->visitReturn_stmt(this);
  else
    return visitor->visitChildren(this);
}

ifccParser::Return_stmtContext* ifccParser::return_stmt() {
  Return_stmtContext *_localctx = _tracker.createInstance<Return_stmtContext>(_ctx, getState());
  enterRule(_localctx, 14, ifccParser::RuleReturn_stmt);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(79);
    match(ifccParser::RETURN);
    setState(80);
    expr();
    setState(81);
    match(ifccParser::SEMI);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExprContext ------------------------------------------------------------------

ifccParser::ExprContext::ExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ifccParser::OrExprContext* ifccParser::ExprContext::orExpr() {
  return getRuleContext<ifccParser::OrExprContext>(0);
}


size_t ifccParser::ExprContext::getRuleIndex() const {
  return ifccParser::RuleExpr;
}


std::any ifccParser::ExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ifccVisitor*>(visitor))
    return parserVisitor->visitExpr(this);
  else
    return visitor->visitChildren(this);
}

ifccParser::ExprContext* ifccParser::expr() {
  ExprContext *_localctx = _tracker.createInstance<ExprContext>(_ctx, getState());
  enterRule(_localctx, 16, ifccParser::RuleExpr);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(83);
    orExpr();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- OrExprContext ------------------------------------------------------------------

ifccParser::OrExprContext::OrExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ifccParser::XorExprContext *> ifccParser::OrExprContext::xorExpr() {
  return getRuleContexts<ifccParser::XorExprContext>();
}

ifccParser::XorExprContext* ifccParser::OrExprContext::xorExpr(size_t i) {
  return getRuleContext<ifccParser::XorExprContext>(i);
}

std::vector<tree::TerminalNode *> ifccParser::OrExprContext::BOR() {
  return getTokens(ifccParser::BOR);
}

tree::TerminalNode* ifccParser::OrExprContext::BOR(size_t i) {
  return getToken(ifccParser::BOR, i);
}


size_t ifccParser::OrExprContext::getRuleIndex() const {
  return ifccParser::RuleOrExpr;
}


std::any ifccParser::OrExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ifccVisitor*>(visitor))
    return parserVisitor->visitOrExpr(this);
  else
    return visitor->visitChildren(this);
}

ifccParser::OrExprContext* ifccParser::orExpr() {
  OrExprContext *_localctx = _tracker.createInstance<OrExprContext>(_ctx, getState());
  enterRule(_localctx, 18, ifccParser::RuleOrExpr);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(85);
    xorExpr();
    setState(90);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ifccParser::BOR) {
      setState(86);
      antlrcpp::downCast<OrExprContext *>(_localctx)->op = match(ifccParser::BOR);
      setState(87);
      xorExpr();
      setState(92);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- XorExprContext ------------------------------------------------------------------

ifccParser::XorExprContext::XorExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ifccParser::AndExprContext *> ifccParser::XorExprContext::andExpr() {
  return getRuleContexts<ifccParser::AndExprContext>();
}

ifccParser::AndExprContext* ifccParser::XorExprContext::andExpr(size_t i) {
  return getRuleContext<ifccParser::AndExprContext>(i);
}

std::vector<tree::TerminalNode *> ifccParser::XorExprContext::BXOR() {
  return getTokens(ifccParser::BXOR);
}

tree::TerminalNode* ifccParser::XorExprContext::BXOR(size_t i) {
  return getToken(ifccParser::BXOR, i);
}


size_t ifccParser::XorExprContext::getRuleIndex() const {
  return ifccParser::RuleXorExpr;
}


std::any ifccParser::XorExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ifccVisitor*>(visitor))
    return parserVisitor->visitXorExpr(this);
  else
    return visitor->visitChildren(this);
}

ifccParser::XorExprContext* ifccParser::xorExpr() {
  XorExprContext *_localctx = _tracker.createInstance<XorExprContext>(_ctx, getState());
  enterRule(_localctx, 20, ifccParser::RuleXorExpr);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(93);
    andExpr();
    setState(98);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ifccParser::BXOR) {
      setState(94);
      antlrcpp::downCast<XorExprContext *>(_localctx)->op = match(ifccParser::BXOR);
      setState(95);
      andExpr();
      setState(100);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AndExprContext ------------------------------------------------------------------

ifccParser::AndExprContext::AndExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ifccParser::EqExprContext *> ifccParser::AndExprContext::eqExpr() {
  return getRuleContexts<ifccParser::EqExprContext>();
}

ifccParser::EqExprContext* ifccParser::AndExprContext::eqExpr(size_t i) {
  return getRuleContext<ifccParser::EqExprContext>(i);
}

std::vector<tree::TerminalNode *> ifccParser::AndExprContext::BAND() {
  return getTokens(ifccParser::BAND);
}

tree::TerminalNode* ifccParser::AndExprContext::BAND(size_t i) {
  return getToken(ifccParser::BAND, i);
}


size_t ifccParser::AndExprContext::getRuleIndex() const {
  return ifccParser::RuleAndExpr;
}


std::any ifccParser::AndExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ifccVisitor*>(visitor))
    return parserVisitor->visitAndExpr(this);
  else
    return visitor->visitChildren(this);
}

ifccParser::AndExprContext* ifccParser::andExpr() {
  AndExprContext *_localctx = _tracker.createInstance<AndExprContext>(_ctx, getState());
  enterRule(_localctx, 22, ifccParser::RuleAndExpr);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(101);
    eqExpr();
    setState(106);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ifccParser::BAND) {
      setState(102);
      antlrcpp::downCast<AndExprContext *>(_localctx)->op = match(ifccParser::BAND);
      setState(103);
      eqExpr();
      setState(108);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- EqExprContext ------------------------------------------------------------------

ifccParser::EqExprContext::EqExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ifccParser::RelExprContext *> ifccParser::EqExprContext::relExpr() {
  return getRuleContexts<ifccParser::RelExprContext>();
}

ifccParser::RelExprContext* ifccParser::EqExprContext::relExpr(size_t i) {
  return getRuleContext<ifccParser::RelExprContext>(i);
}

std::vector<tree::TerminalNode *> ifccParser::EqExprContext::EQ() {
  return getTokens(ifccParser::EQ);
}

tree::TerminalNode* ifccParser::EqExprContext::EQ(size_t i) {
  return getToken(ifccParser::EQ, i);
}

std::vector<tree::TerminalNode *> ifccParser::EqExprContext::NEQ() {
  return getTokens(ifccParser::NEQ);
}

tree::TerminalNode* ifccParser::EqExprContext::NEQ(size_t i) {
  return getToken(ifccParser::NEQ, i);
}


size_t ifccParser::EqExprContext::getRuleIndex() const {
  return ifccParser::RuleEqExpr;
}


std::any ifccParser::EqExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ifccVisitor*>(visitor))
    return parserVisitor->visitEqExpr(this);
  else
    return visitor->visitChildren(this);
}

ifccParser::EqExprContext* ifccParser::eqExpr() {
  EqExprContext *_localctx = _tracker.createInstance<EqExprContext>(_ctx, getState());
  enterRule(_localctx, 24, ifccParser::RuleEqExpr);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(109);
    relExpr();
    setState(114);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ifccParser::EQ

    || _la == ifccParser::NEQ) {
      setState(110);
      antlrcpp::downCast<EqExprContext *>(_localctx)->op = _input->LT(1);
      _la = _input->LA(1);
      if (!(_la == ifccParser::EQ

      || _la == ifccParser::NEQ)) {
        antlrcpp::downCast<EqExprContext *>(_localctx)->op = _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(111);
      relExpr();
      setState(116);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- RelExprContext ------------------------------------------------------------------

ifccParser::RelExprContext::RelExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ifccParser::AddExprContext *> ifccParser::RelExprContext::addExpr() {
  return getRuleContexts<ifccParser::AddExprContext>();
}

ifccParser::AddExprContext* ifccParser::RelExprContext::addExpr(size_t i) {
  return getRuleContext<ifccParser::AddExprContext>(i);
}

tree::TerminalNode* ifccParser::RelExprContext::LT() {
  return getToken(ifccParser::LT, 0);
}

tree::TerminalNode* ifccParser::RelExprContext::GT() {
  return getToken(ifccParser::GT, 0);
}


size_t ifccParser::RelExprContext::getRuleIndex() const {
  return ifccParser::RuleRelExpr;
}


std::any ifccParser::RelExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ifccVisitor*>(visitor))
    return parserVisitor->visitRelExpr(this);
  else
    return visitor->visitChildren(this);
}

ifccParser::RelExprContext* ifccParser::relExpr() {
  RelExprContext *_localctx = _tracker.createInstance<RelExprContext>(_ctx, getState());
  enterRule(_localctx, 26, ifccParser::RuleRelExpr);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(117);
    addExpr();
    setState(120);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ifccParser::LT

    || _la == ifccParser::GT) {
      setState(118);
      antlrcpp::downCast<RelExprContext *>(_localctx)->op = _input->LT(1);
      _la = _input->LA(1);
      if (!(_la == ifccParser::LT

      || _la == ifccParser::GT)) {
        antlrcpp::downCast<RelExprContext *>(_localctx)->op = _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(119);
      addExpr();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AddExprContext ------------------------------------------------------------------

ifccParser::AddExprContext::AddExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ifccParser::MulExprContext *> ifccParser::AddExprContext::mulExpr() {
  return getRuleContexts<ifccParser::MulExprContext>();
}

ifccParser::MulExprContext* ifccParser::AddExprContext::mulExpr(size_t i) {
  return getRuleContext<ifccParser::MulExprContext>(i);
}

std::vector<tree::TerminalNode *> ifccParser::AddExprContext::PLUS() {
  return getTokens(ifccParser::PLUS);
}

tree::TerminalNode* ifccParser::AddExprContext::PLUS(size_t i) {
  return getToken(ifccParser::PLUS, i);
}

std::vector<tree::TerminalNode *> ifccParser::AddExprContext::MINUS() {
  return getTokens(ifccParser::MINUS);
}

tree::TerminalNode* ifccParser::AddExprContext::MINUS(size_t i) {
  return getToken(ifccParser::MINUS, i);
}


size_t ifccParser::AddExprContext::getRuleIndex() const {
  return ifccParser::RuleAddExpr;
}


std::any ifccParser::AddExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ifccVisitor*>(visitor))
    return parserVisitor->visitAddExpr(this);
  else
    return visitor->visitChildren(this);
}

ifccParser::AddExprContext* ifccParser::addExpr() {
  AddExprContext *_localctx = _tracker.createInstance<AddExprContext>(_ctx, getState());
  enterRule(_localctx, 28, ifccParser::RuleAddExpr);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(122);
    mulExpr();
    setState(127);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ifccParser::PLUS

    || _la == ifccParser::MINUS) {
      setState(123);
      antlrcpp::downCast<AddExprContext *>(_localctx)->op = _input->LT(1);
      _la = _input->LA(1);
      if (!(_la == ifccParser::PLUS

      || _la == ifccParser::MINUS)) {
        antlrcpp::downCast<AddExprContext *>(_localctx)->op = _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(124);
      mulExpr();
      setState(129);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- MulExprContext ------------------------------------------------------------------

ifccParser::MulExprContext::MulExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ifccParser::UnaryExprContext *> ifccParser::MulExprContext::unaryExpr() {
  return getRuleContexts<ifccParser::UnaryExprContext>();
}

ifccParser::UnaryExprContext* ifccParser::MulExprContext::unaryExpr(size_t i) {
  return getRuleContext<ifccParser::UnaryExprContext>(i);
}

std::vector<tree::TerminalNode *> ifccParser::MulExprContext::STAR() {
  return getTokens(ifccParser::STAR);
}

tree::TerminalNode* ifccParser::MulExprContext::STAR(size_t i) {
  return getToken(ifccParser::STAR, i);
}

std::vector<tree::TerminalNode *> ifccParser::MulExprContext::SLASH() {
  return getTokens(ifccParser::SLASH);
}

tree::TerminalNode* ifccParser::MulExprContext::SLASH(size_t i) {
  return getToken(ifccParser::SLASH, i);
}

std::vector<tree::TerminalNode *> ifccParser::MulExprContext::PERCENT() {
  return getTokens(ifccParser::PERCENT);
}

tree::TerminalNode* ifccParser::MulExprContext::PERCENT(size_t i) {
  return getToken(ifccParser::PERCENT, i);
}


size_t ifccParser::MulExprContext::getRuleIndex() const {
  return ifccParser::RuleMulExpr;
}


std::any ifccParser::MulExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ifccVisitor*>(visitor))
    return parserVisitor->visitMulExpr(this);
  else
    return visitor->visitChildren(this);
}

ifccParser::MulExprContext* ifccParser::mulExpr() {
  MulExprContext *_localctx = _tracker.createInstance<MulExprContext>(_ctx, getState());
  enterRule(_localctx, 30, ifccParser::RuleMulExpr);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(130);
    unaryExpr();
    setState(135);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 14336) != 0)) {
      setState(131);
      antlrcpp::downCast<MulExprContext *>(_localctx)->op = _input->LT(1);
      _la = _input->LA(1);
      if (!((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & 14336) != 0))) {
        antlrcpp::downCast<MulExprContext *>(_localctx)->op = _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(132);
      unaryExpr();
      setState(137);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- UnaryExprContext ------------------------------------------------------------------

ifccParser::UnaryExprContext::UnaryExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ifccParser::UnaryExprContext* ifccParser::UnaryExprContext::unaryExpr() {
  return getRuleContext<ifccParser::UnaryExprContext>(0);
}

tree::TerminalNode* ifccParser::UnaryExprContext::NOT() {
  return getToken(ifccParser::NOT, 0);
}

tree::TerminalNode* ifccParser::UnaryExprContext::MINUS() {
  return getToken(ifccParser::MINUS, 0);
}

ifccParser::PrimaryContext* ifccParser::UnaryExprContext::primary() {
  return getRuleContext<ifccParser::PrimaryContext>(0);
}


size_t ifccParser::UnaryExprContext::getRuleIndex() const {
  return ifccParser::RuleUnaryExpr;
}


std::any ifccParser::UnaryExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ifccVisitor*>(visitor))
    return parserVisitor->visitUnaryExpr(this);
  else
    return visitor->visitChildren(this);
}

ifccParser::UnaryExprContext* ifccParser::unaryExpr() {
  UnaryExprContext *_localctx = _tracker.createInstance<UnaryExprContext>(_ctx, getState());
  enterRule(_localctx, 32, ifccParser::RuleUnaryExpr);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(141);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ifccParser::MINUS:
      case ifccParser::NOT: {
        enterOuterAlt(_localctx, 1);
        setState(138);
        antlrcpp::downCast<UnaryExprContext *>(_localctx)->op = _input->LT(1);
        _la = _input->LA(1);
        if (!(_la == ifccParser::MINUS

        || _la == ifccParser::NOT)) {
          antlrcpp::downCast<UnaryExprContext *>(_localctx)->op = _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        setState(139);
        unaryExpr();
        break;
      }

      case ifccParser::LPAREN:
      case ifccParser::CONST:
      case ifccParser::VAR: {
        enterOuterAlt(_localctx, 2);
        setState(140);
        primary();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PrimaryContext ------------------------------------------------------------------

ifccParser::PrimaryContext::PrimaryContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ifccParser::PrimaryContext::CONST() {
  return getToken(ifccParser::CONST, 0);
}

tree::TerminalNode* ifccParser::PrimaryContext::VAR() {
  return getToken(ifccParser::VAR, 0);
}

tree::TerminalNode* ifccParser::PrimaryContext::LPAREN() {
  return getToken(ifccParser::LPAREN, 0);
}

ifccParser::ExprContext* ifccParser::PrimaryContext::expr() {
  return getRuleContext<ifccParser::ExprContext>(0);
}

tree::TerminalNode* ifccParser::PrimaryContext::RPAREN() {
  return getToken(ifccParser::RPAREN, 0);
}


size_t ifccParser::PrimaryContext::getRuleIndex() const {
  return ifccParser::RulePrimary;
}


std::any ifccParser::PrimaryContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ifccVisitor*>(visitor))
    return parserVisitor->visitPrimary(this);
  else
    return visitor->visitChildren(this);
}

ifccParser::PrimaryContext* ifccParser::primary() {
  PrimaryContext *_localctx = _tracker.createInstance<PrimaryContext>(_ctx, getState());
  enterRule(_localctx, 34, ifccParser::RulePrimary);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(149);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ifccParser::CONST: {
        enterOuterAlt(_localctx, 1);
        setState(143);
        match(ifccParser::CONST);
        break;
      }

      case ifccParser::VAR: {
        enterOuterAlt(_localctx, 2);
        setState(144);
        match(ifccParser::VAR);
        break;
      }

      case ifccParser::LPAREN: {
        enterOuterAlt(_localctx, 3);
        setState(145);
        match(ifccParser::LPAREN);
        setState(146);
        expr();
        setState(147);
        match(ifccParser::RPAREN);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

void ifccParser::initialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  ifccParserInitialize();
#else
  ::antlr4::internal::call_once(ifccParserOnceFlag, ifccParserInitialize);
#endif
}
