
// Generated from ifcc.g4 by ANTLR 4.13.2


#include "ifccLexer.h"


using namespace antlr4;



using namespace antlr4;

namespace {

struct IfccLexerStaticData final {
  IfccLexerStaticData(std::vector<std::string> ruleNames,
                          std::vector<std::string> channelNames,
                          std::vector<std::string> modeNames,
                          std::vector<std::string> literalNames,
                          std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), channelNames(std::move(channelNames)),
        modeNames(std::move(modeNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  IfccLexerStaticData(const IfccLexerStaticData&) = delete;
  IfccLexerStaticData(IfccLexerStaticData&&) = delete;
  IfccLexerStaticData& operator=(const IfccLexerStaticData&) = delete;
  IfccLexerStaticData& operator=(IfccLexerStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> channelNames;
  const std::vector<std::string> modeNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

::antlr4::internal::OnceFlag ifcclexerLexerOnceFlag;
#if ANTLR4_USE_THREAD_LOCAL_CACHE
static thread_local
#endif
std::unique_ptr<IfccLexerStaticData> ifcclexerLexerStaticData = nullptr;

void ifcclexerLexerInitialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  if (ifcclexerLexerStaticData != nullptr) {
    return;
  }
#else
  assert(ifcclexerLexerStaticData == nullptr);
#endif
  auto staticData = std::make_unique<IfccLexerStaticData>(
    std::vector<std::string>{
      "INT", "RETURN", "MAIN", "EQ", "NEQ", "ASSIGN", "LT", "GT", "PLUS", 
      "MINUS", "STAR", "SLASH", "PERCENT", "BAND", "BXOR", "BOR", "NOT", 
      "LPAREN", "RPAREN", "LBRACE", "RBRACE", "SEMI", "COMMA", "CONST", 
      "VAR", "COMMENT", "DIRECTIVE", "WS"
    },
    std::vector<std::string>{
      "DEFAULT_TOKEN_CHANNEL", "HIDDEN"
    },
    std::vector<std::string>{
      "DEFAULT_MODE"
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
  	4,0,28,159,6,-1,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,
  	6,2,7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,2,14,
  	7,14,2,15,7,15,2,16,7,16,2,17,7,17,2,18,7,18,2,19,7,19,2,20,7,20,2,21,
  	7,21,2,22,7,22,2,23,7,23,2,24,7,24,2,25,7,25,2,26,7,26,2,27,7,27,1,0,
  	1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1,2,1,2,1,2,1,2,1,3,1,3,1,
  	3,1,4,1,4,1,4,1,5,1,5,1,6,1,6,1,7,1,7,1,8,1,8,1,9,1,9,1,10,1,10,1,11,
  	1,11,1,12,1,12,1,13,1,13,1,14,1,14,1,15,1,15,1,16,1,16,1,17,1,17,1,18,
  	1,18,1,19,1,19,1,20,1,20,1,21,1,21,1,22,1,22,1,23,4,23,117,8,23,11,23,
  	12,23,118,1,24,1,24,5,24,123,8,24,10,24,12,24,126,9,24,1,25,1,25,1,25,
  	1,25,5,25,132,8,25,10,25,12,25,135,9,25,1,25,1,25,1,25,1,25,1,25,1,26,
  	1,26,5,26,144,8,26,10,26,12,26,147,9,26,1,26,1,26,1,26,1,26,1,27,4,27,
  	154,8,27,11,27,12,27,155,1,27,1,27,2,133,145,0,28,1,1,3,2,5,3,7,4,9,5,
  	11,6,13,7,15,8,17,9,19,10,21,11,23,12,25,13,27,14,29,15,31,16,33,17,35,
  	18,37,19,39,20,41,21,43,22,45,23,47,24,49,25,51,26,53,27,55,28,1,0,4,
  	1,0,48,57,3,0,65,90,95,95,97,122,4,0,48,57,65,90,95,95,97,122,3,0,9,10,
  	13,13,32,32,163,0,1,1,0,0,0,0,3,1,0,0,0,0,5,1,0,0,0,0,7,1,0,0,0,0,9,1,
  	0,0,0,0,11,1,0,0,0,0,13,1,0,0,0,0,15,1,0,0,0,0,17,1,0,0,0,0,19,1,0,0,
  	0,0,21,1,0,0,0,0,23,1,0,0,0,0,25,1,0,0,0,0,27,1,0,0,0,0,29,1,0,0,0,0,
  	31,1,0,0,0,0,33,1,0,0,0,0,35,1,0,0,0,0,37,1,0,0,0,0,39,1,0,0,0,0,41,1,
  	0,0,0,0,43,1,0,0,0,0,45,1,0,0,0,0,47,1,0,0,0,0,49,1,0,0,0,0,51,1,0,0,
  	0,0,53,1,0,0,0,0,55,1,0,0,0,1,57,1,0,0,0,3,61,1,0,0,0,5,68,1,0,0,0,7,
  	73,1,0,0,0,9,76,1,0,0,0,11,79,1,0,0,0,13,81,1,0,0,0,15,83,1,0,0,0,17,
  	85,1,0,0,0,19,87,1,0,0,0,21,89,1,0,0,0,23,91,1,0,0,0,25,93,1,0,0,0,27,
  	95,1,0,0,0,29,97,1,0,0,0,31,99,1,0,0,0,33,101,1,0,0,0,35,103,1,0,0,0,
  	37,105,1,0,0,0,39,107,1,0,0,0,41,109,1,0,0,0,43,111,1,0,0,0,45,113,1,
  	0,0,0,47,116,1,0,0,0,49,120,1,0,0,0,51,127,1,0,0,0,53,141,1,0,0,0,55,
  	153,1,0,0,0,57,58,5,105,0,0,58,59,5,110,0,0,59,60,5,116,0,0,60,2,1,0,
  	0,0,61,62,5,114,0,0,62,63,5,101,0,0,63,64,5,116,0,0,64,65,5,117,0,0,65,
  	66,5,114,0,0,66,67,5,110,0,0,67,4,1,0,0,0,68,69,5,109,0,0,69,70,5,97,
  	0,0,70,71,5,105,0,0,71,72,5,110,0,0,72,6,1,0,0,0,73,74,5,61,0,0,74,75,
  	5,61,0,0,75,8,1,0,0,0,76,77,5,33,0,0,77,78,5,61,0,0,78,10,1,0,0,0,79,
  	80,5,61,0,0,80,12,1,0,0,0,81,82,5,60,0,0,82,14,1,0,0,0,83,84,5,62,0,0,
  	84,16,1,0,0,0,85,86,5,43,0,0,86,18,1,0,0,0,87,88,5,45,0,0,88,20,1,0,0,
  	0,89,90,5,42,0,0,90,22,1,0,0,0,91,92,5,47,0,0,92,24,1,0,0,0,93,94,5,37,
  	0,0,94,26,1,0,0,0,95,96,5,38,0,0,96,28,1,0,0,0,97,98,5,94,0,0,98,30,1,
  	0,0,0,99,100,5,124,0,0,100,32,1,0,0,0,101,102,5,33,0,0,102,34,1,0,0,0,
  	103,104,5,40,0,0,104,36,1,0,0,0,105,106,5,41,0,0,106,38,1,0,0,0,107,108,
  	5,123,0,0,108,40,1,0,0,0,109,110,5,125,0,0,110,42,1,0,0,0,111,112,5,59,
  	0,0,112,44,1,0,0,0,113,114,5,44,0,0,114,46,1,0,0,0,115,117,7,0,0,0,116,
  	115,1,0,0,0,117,118,1,0,0,0,118,116,1,0,0,0,118,119,1,0,0,0,119,48,1,
  	0,0,0,120,124,7,1,0,0,121,123,7,2,0,0,122,121,1,0,0,0,123,126,1,0,0,0,
  	124,122,1,0,0,0,124,125,1,0,0,0,125,50,1,0,0,0,126,124,1,0,0,0,127,128,
  	5,47,0,0,128,129,5,42,0,0,129,133,1,0,0,0,130,132,9,0,0,0,131,130,1,0,
  	0,0,132,135,1,0,0,0,133,134,1,0,0,0,133,131,1,0,0,0,134,136,1,0,0,0,135,
  	133,1,0,0,0,136,137,5,42,0,0,137,138,5,47,0,0,138,139,1,0,0,0,139,140,
  	6,25,0,0,140,52,1,0,0,0,141,145,5,35,0,0,142,144,9,0,0,0,143,142,1,0,
  	0,0,144,147,1,0,0,0,145,146,1,0,0,0,145,143,1,0,0,0,146,148,1,0,0,0,147,
  	145,1,0,0,0,148,149,5,10,0,0,149,150,1,0,0,0,150,151,6,26,0,0,151,54,
  	1,0,0,0,152,154,7,3,0,0,153,152,1,0,0,0,154,155,1,0,0,0,155,153,1,0,0,
  	0,155,156,1,0,0,0,156,157,1,0,0,0,157,158,6,27,0,0,158,56,1,0,0,0,6,0,
  	118,124,133,145,155,1,6,0,0
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  ifcclexerLexerStaticData = std::move(staticData);
}

}

ifccLexer::ifccLexer(CharStream *input) : Lexer(input) {
  ifccLexer::initialize();
  _interpreter = new atn::LexerATNSimulator(this, *ifcclexerLexerStaticData->atn, ifcclexerLexerStaticData->decisionToDFA, ifcclexerLexerStaticData->sharedContextCache);
}

ifccLexer::~ifccLexer() {
  delete _interpreter;
}

std::string ifccLexer::getGrammarFileName() const {
  return "ifcc.g4";
}

const std::vector<std::string>& ifccLexer::getRuleNames() const {
  return ifcclexerLexerStaticData->ruleNames;
}

const std::vector<std::string>& ifccLexer::getChannelNames() const {
  return ifcclexerLexerStaticData->channelNames;
}

const std::vector<std::string>& ifccLexer::getModeNames() const {
  return ifcclexerLexerStaticData->modeNames;
}

const dfa::Vocabulary& ifccLexer::getVocabulary() const {
  return ifcclexerLexerStaticData->vocabulary;
}

antlr4::atn::SerializedATNView ifccLexer::getSerializedATN() const {
  return ifcclexerLexerStaticData->serializedATN;
}

const atn::ATN& ifccLexer::getATN() const {
  return *ifcclexerLexerStaticData->atn;
}




void ifccLexer::initialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  ifcclexerLexerInitialize();
#else
  ::antlr4::internal::call_once(ifcclexerLexerOnceFlag, ifcclexerLexerInitialize);
#endif
}
