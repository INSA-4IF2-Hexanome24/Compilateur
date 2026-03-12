grammar ifcc;

axiom
  : prog EOF
  ;

prog
  : 'int' 'main' '(' ')' '{' stmt* return_stmt '}'
  ;

// ----------------------
// Statements
// ----------------------

stmt
  : decl_stmt
  | assign_stmt
  | if_stmt
  | block
  ;

decl_stmt
  : 'int' var_decl_list ';'
  ;

var_decl_list
  : var_decl (',' var_decl)*
  ;

var_decl
  : VAR
  | VAR '=' expr
  ;

assign_stmt
  : VAR '=' expr ';'
  ;

return_stmt
  : 'return' expr ';'
  ;

if_stmt
  : 'if' '(' expr ')' block
    ('else' block)?
  ;

block
  : '{' stmt* '}'
  ;


// ----------------------
// Expressions
// ----------------------


//attention a l'ordre (inverse)
//https://en.cppreference.com/w/c/language/operator_precedence.html 
//exemple meme niveau OP=('*'|'%')

expr
  : '-' expr                           # unaryMinus
  | '!' expr                           # notExpr
  | expr  OP=('*'|'/'|'%') expr        # Multdivmod
  | expr  OP=('+'|'-') expr            # Plusminus
  | expr  OP=('<'|'>') expr            # Ltgt 
  | expr  OP=('=='|'!=') expr          # Eqneq
  | expr '&' expr                      # band
  | expr '^' expr                      # bxor
  | expr '|' expr                      # bor 
  | '(' expr ')'                       # parens
  | CONST                              # constExpr
  | VAR                                # varExpr
  ;

// ----------------------
// Lexer
// ----------------------

CONST : [0-9]+;
VAR   : [a-zA-Z_][a-zA-Z_0-9]*;

COMMENT   : '/*' .*? '*/' -> skip;
DIRECTIVE : '#' .*? '\n'  -> skip;
WS        : [ \t\r\n]+ -> skip;