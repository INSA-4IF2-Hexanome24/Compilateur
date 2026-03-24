grammar ifcc;

axiom
  : prog EOF
  ;

prog
  : function_def+
  ;

function_def
  : type_spec VAR '(' param_list? ')' block
  ;

type_spec
  : 'int'
  | 'void'
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
  | break_stmt       
  | continue_stmt    
  | switch_stmt
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
  : '*'? VAR
  | '*'? VAR '=' expr
  ;

assign_stmt
  : VAR '=' expr ';'  # assignSimple_stmt
  | VAR '+=' expr ';' # addAssign_stmt
  | VAR '-=' expr ';' # minusAssign_stmt
  | VAR '*=' expr ';' # multAssign_stmt
  | VAR '/=' expr ';' # divAssign_stmt
  | '--' VAR ';' # preMinus_stmt
  | '++' VAR ';' # preAdd_stmt
  | VAR '--' ';' # postMinus_stmt
  | VAR '++' ';' # postAdd_stmt
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

break_stmt
  : 'break' ';'
  ;

continue_stmt
  : 'continue' ';'
  ;

switch_stmt
  : 'switch' '(' expr ')' '{' case_clause* default_clause? '}'
  ;

case_clause
  : 'case' CONST ':' stmt*
  ;

default_clause
  : 'default' ':' stmt*
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
  | expr '&&' expr                     # AndCond
  | expr '||' expr                     # OrCond
  | '(' expr ')'                       # parens
  | CONST                              # constExpr
  | '&' VAR                            # addrOfVar
  | VAR                                # varExpr
  | VAR '(' arg_list? ')'              # funcCall
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
