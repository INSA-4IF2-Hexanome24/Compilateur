grammar ifcc;

axiom : prog EOF ;

prog
  : INT MAIN LPAREN RPAREN LBRACE stmt* return_stmt RBRACE
  ;

stmt
  : decl_stmt
  | assign_stmt
  ;

decl_stmt
  : INT VAR SEMI
  ;

assign_stmt
  : VAR ASSIGN expr SEMI
  ;

return_stmt
  : RETURN expr SEMI
  ;

// ----------------------
// Expressions (precedence)
// ----------------------

expr
  : orExpr
  ;

orExpr
  : xorExpr (op=BOR xorExpr)*
  ;

xorExpr
  : andExpr (op=BXOR andExpr)*
  ;

andExpr
  : eqExpr (op=BAND eqExpr)*
  ;

eqExpr
  : relExpr (op=(EQ | NEQ) relExpr)*
  ;

relExpr
  : addExpr (op=(LT | GT) addExpr)*
  ;

addExpr
  : mulExpr (op=(PLUS | MINUS) mulExpr)*
  ;

mulExpr
  : unaryExpr (op=(STAR | SLASH | PERCENT) unaryExpr)*
  ;

unaryExpr
  : op=(NOT | MINUS) unaryExpr
  | primary
  ;

primary
  : CONST
  | VAR
  | LPAREN expr RPAREN
  ;

// ----------------------
// Lexer
// ----------------------

INT    : 'int';
RETURN : 'return';
MAIN   : 'main';

EQ     : '==';
NEQ    : '!=';
ASSIGN : '=';

LT     : '<';
GT     : '>';

PLUS   : '+';
MINUS  : '-';
STAR   : '*';
SLASH  : '/';
PERCENT: '%';

BAND   : '&';
BXOR   : '^';
BOR    : '|';

NOT    : '!';

LPAREN : '(';
RPAREN : ')';
LBRACE : '{';
RBRACE : '}';
SEMI   : ';';

CONST  : [0-9]+;
VAR    : [a-zA-Z_][a-zA-Z_0-9]*;

COMMENT   : '/*' .*? '*/' -> skip;
DIRECTIVE : '#' .*? '\n'  -> skip;
WS        : [ \t\r\n]     -> skip;
