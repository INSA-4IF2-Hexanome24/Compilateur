grammar ifcc;

axiom : prog EOF ;

prog
  : 'int' 'main' '(' ')' '{' stmt* return_stmt '}'
  ;

stmt
  : decl_stmt
  | assign_stmt
  ;

// Declaration with optional initialization: int a; or int a = 42;
decl_stmt
  : 'int' var_decl_list ';'
  ;

// Liste de variables, chacune optionnellement initialisée
var_decl_list
  : var_decl (',' var_decl)*
  ;

var_decl
  : VAR ('=' expr)?
  ;

assign_stmt
  : VAR '=' expr ';'
  ;

return_stmt
  : 'return' expr ';'
  ;

// ----------------------
// Expressions (precedence)
// ----------------------

expr
  : orExpr
  ;

orExpr
  : xorExpr (op='|' xorExpr)*
  ;

xorExpr
  : andExpr (op='^' andExpr)*
  ;

andExpr
  : eqExpr (op='&' eqExpr)*
  ;

eqExpr
  : relExpr (op=('==' | '!=') relExpr)*
  ;

relExpr
  : addExpr (op=('<' | '>') addExpr)?
  ;
  
addExpr
  : mulExpr (op=('+' | '-') mulExpr)*
  ;

mulExpr
  : unaryExpr (op=('*' | '/' | '%') unaryExpr)*
  ;

unaryExpr
  : op=('!' | '-') unaryExpr
  | primary
  ;

primary
  : CONST
  | VAR
  | '(' expr ')'
  ;

// ----------------------
// Lexer
// ----------------------

CONST  : [0-9]+;
VAR    : [a-zA-Z_][a-zA-Z_0-9]*;

COMMENT   : '/*' .*? '*/' -> skip;
DIRECTIVE : '#' .*? '\n'  -> skip;
WS        : [ \t\r\n]+-> skip;