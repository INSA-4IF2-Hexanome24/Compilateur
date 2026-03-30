grammar ifcc;

axiom
  : prog EOF
  ;

prog
  : function_def+
  ;

function_def
  : 'int' VAR '(' param_list? ')' block
  ;

param_list
  : param (',' param)*
  ;

param
  : 'int' '*'? VAR
  ;

// ----------------------
// Statements
// ----------------------

stmt
  : decl_stmt
  | assign_stmt
  | ptr_assign_stmt
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
  | tab
  ;

var_decl_list
  : var_decl (',' var_decl)*
  ;

tab
  : 'int' VAR '[' CONST ']' ';'
  ;

var_decl
  : '*'? VAR
  | '*'? VAR '=' expr
  ;

assign_stmt
  : VAR '=' expr ';'              # SimpleAssign
  | VAR '[' expr ']' '=' expr ';' # ArrayWrite
  ;

ptr_assign_stmt
  : '*' VAR '=' expr ';'
  ;

return_stmt
  : 'return' expr? ';'
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
  | '*' expr                           # derefExpr
  | expr  OP=('*'|'/'|'%') expr        # Multdivmod
  | expr  OP=('+'|'-') expr            # Plusminus
  | expr  OP=('<'|'>') expr            # Ltgt 
  | expr  OP=('=='|'!=') expr          # Eqneq
  | expr '&' expr                      # band
  | expr '^' expr                      # bxor
  | expr '|' expr                      # bor 
  | '(' expr ')'                       # parens
  | CONST                              # constExpr
  | '&' VAR                            # addrOfVar
  | VAR '(' arg_list? ')'              # funcCall
  | VAR '[' expr ']'                   # ArrayRead
  | VAR                                # varExpr
  ;

// ----------------------
// Lexer
// ----------------------

CONST : [0-9]+;
VAR   : [a-zA-Z_][a-zA-Z_0-9]*;

COMMENT   : '/*' .*? '*/' -> skip;
LINE_COMMENT : '//' ~[\r\n]* -> skip;
DIRECTIVE : '#' .*? '\n'  -> skip;
WS        : [ \t\r\n]+ -> skip;
