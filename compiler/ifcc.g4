grammar ifcc;

axiom : prog EOF ;

prog
  : INT MAIN LPAREN RPAREN LBRACE stmt* return_stmt RBRACE
  ;

// ----------------------
// Statements
// ----------------------

stmt
  : decl_stmt
  | assign_stmt
  ;

decl_stmt
  : INT var_decl_list SEMI
  ;

var_decl_list
  : var_decl (COMMA var_decl)*
  ;

var_decl
  : VAR
  | VAR ASSIGN expr
  ;

assign_stmt
  : VAR ASSIGN expr SEMI
  ;

return_stmt
  : RETURN expr SEMI
  ;

// ----------------------
// Expressions
// ----------------------

expr
  : expr STAR expr        # mult        // multiplication
  | expr SLASH expr       # div         // division
  | expr PERCENT expr     # mod         // modulo
  | expr PLUS expr        # plus        // addition
  | expr MINUS expr       # minus       // soustraction
  | expr LT expr          # lt          // inférieur <
  | expr GT expr          # gt          // supérieur >
  | expr EQ expr          # eq          // égal ==
  | expr NEQ expr         # neq         // différent !=
  | expr BAND expr        # band        // AND bit à bit &
  | expr BXOR expr        # bxor        // XOR bit à bit ^
  | expr BOR expr         # bor         // OR bit à bit |
  | NOT expr              # notExpr     // négation !
  | MINUS expr            # unaryMinus  // négation uniaire
  | LPAREN expr RPAREN    # parens      // parenthèses ( )
  | CONST                 # constExpr   // constante entière
  | VAR                   # varExpr     // variable
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
COMMA  : ',';

CONST  : [0-9]+;
VAR    : [a-zA-Z_][a-zA-Z_0-9]*;

COMMENT   : '/*' .*? '*/' -> skip;
DIRECTIVE : '#' .*? '\n'  -> skip;
WS        : [ \t\r\n]+ -> skip;