
// Generated from ifcc.g4 by ANTLR 4.13.2

#pragma once


#include "antlr4-runtime.h"




class  ifccLexer : public antlr4::Lexer {
public:
  enum {
    INT = 1, RETURN = 2, MAIN = 3, EQ = 4, NEQ = 5, ASSIGN = 6, LT = 7, 
    GT = 8, PLUS = 9, MINUS = 10, STAR = 11, SLASH = 12, PERCENT = 13, BAND = 14, 
    BXOR = 15, BOR = 16, NOT = 17, LPAREN = 18, RPAREN = 19, LBRACE = 20, 
    RBRACE = 21, SEMI = 22, COMMA = 23, CONST = 24, VAR = 25, COMMENT = 26, 
    DIRECTIVE = 27, WS = 28
  };

  explicit ifccLexer(antlr4::CharStream *input);

  ~ifccLexer() override;


  std::string getGrammarFileName() const override;

  const std::vector<std::string>& getRuleNames() const override;

  const std::vector<std::string>& getChannelNames() const override;

  const std::vector<std::string>& getModeNames() const override;

  const antlr4::dfa::Vocabulary& getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;

  const antlr4::atn::ATN& getATN() const override;

  // By default the static state used to implement the lexer is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:

  // Individual action functions triggered by action() above.

  // Individual semantic predicate functions triggered by sempred() above.

};

