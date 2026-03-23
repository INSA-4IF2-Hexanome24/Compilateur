grammar ifcc;

axiom
  : prog EOF
  ;

prog
  : function_def+
  ;

function_def
  : 'int' VAR '(' param_list? ')' block
  | 'void' VAR '(' param_list? ')' block
  ;

param_list
  : param (',' param)*
  ;

param
  : 'int' VAR
  ;

// ----------------------
// Statements
// ----------------------

stmt
  : decl_stmt
  | assign_stmt
  | if_stmt
  | expr_stmt
  | block
  | while_stmt
  | return_stmt
  ;

expr_stmt
  : expr ';'
  ;

arg_list
  : expr (',' expr)*
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
  : VAR '=' expr ';'                              # simpleAssign
  | VAR OP=('+='|'-='|'*='|'/='|'%=') expr ';'   # compoundAssign
  | OP=('++'|'--') VAR ';'                        # preIncDec
  | VAR OP=('++'|'--') ';'                        # postIncDec
  ;

return_stmt
  : 'return' expr ';'
  | 'return' ';'
  ;

if_stmt
  : 'if' '(' expr ')' block
    ('else' block)?
  ;

block
  : '{' stmt* '}'
  ;

while_stmt
  : 'while' '(' expr ')' block
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
  | VAR '(' arg_list? ')'              # funcCall
  ;

// ----------------------
// Lexer
// ----------------------

CONST : [0-9]+;
VAR   : [a-zA-Z_][a-zA-Z_0-9]*;

COMMENT   : '/*' .*? '*/' -> skip;
DIRECTIVE : '#' .*? '\n'  -> skip;
WS        : [ \t\r\n]+ -> skip;
