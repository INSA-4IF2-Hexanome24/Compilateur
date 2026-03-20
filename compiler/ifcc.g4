grammar ifcc;

axiom
  : prog EOF
  ;

prog
  : 'int' 'main' '(' ')' '{' stmt* '}'
  ;

// ----------------------
// Statements
// ----------------------

stmt
  : 'int' var_decl_list ';'                   # decl_stmt
  | VAR '=' expr ';'                          # assign_stmt
  | 'if' '(' expr ')' block ('else' block)?   # if_stmt
  | expr ';'                                  # expr_stmt
  | block                                     # block
  | 'while' '(' expr ')' block                # while_stmt
  | 'return' expr ';'                         # return_stmt
  ;



arg_list
  : expr (',' expr)*
  ;

var_decl_list
  : var_decl (',' var_decl)*
  ;

var_decl
  : VAR
  | VAR '=' expr
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
