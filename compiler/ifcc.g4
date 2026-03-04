grammar ifcc;

axiom : prog EOF ;

prog : 'int' 'main' '(' ')' '{' stmt* return_stmt '}' ;

stmt
    : decl_stmt
    | assign_stmt
    ;

decl_stmt   : 'int' VAR ';' ;
assign_stmt : VAR '=' expr ';' ;

return_stmt : RETURN expr ';' ;

expr
    : CONST   # constExpr
    | VAR     # varExpr
    ;

RETURN : 'return' ;
CONST  : [0-9]+ ;
VAR    : [a-zA-Z_][a-zA-Z_0-9]* ;
COMMENT   : '/*' .*? '*/' -> skip ;
DIRECTIVE : '#' .*? '\n'  -> skip ;
WS        : [ \t\r\n]     -> channel(HIDDEN);
