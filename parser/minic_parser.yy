%{
  #include <stdio.h>
  #include <math.h>
  void yyerror (char const *);
%}

%code requires{
  #include "lexer/lexer.hpp"
  #include "parser/parser.hpp"
}
%parse-param { std::istream& yyin };
%parse-param { std::ostream& yyout };
%parse-param { minic::parser::bisonFlexLexer& lexer };
%initial-action
{
  lexer.switch_streams(nullptr, &yyout);
  lexer.yyrestart(&yyin);
}

%code {
  #define yylex lexer.yylakes
}

%token IF_KWD 13 ELSE_KWD 14 WHILE_KWD 15 BREAK_KWD 16
%token CONTINUE_KWD 17 EXIT_KWD 18 PRINT_KWD 19 PRINTLN_KWD 20
%token INT_TYPE 21 DOUBLE_TYPE 22 STRING_TYPE 23 BOOL_TYPE 24
%token MUL_BIN 25 DIV_BIN 26 PLUS_BIN 27 MINUS_BIN 28 MOD_BIN 29
%token LT_BIN 30 GT_BIN 31 LTEQ_BIN 32 GTEQ_BIN 33 ASSIGN_BIN 34
%token EQ_BIN 35 NOTEQ_BIN 36 NOT_UN 37 AND_BIN 38 OR_BIN 39
%token LPAR 40 RPAR 41 LBRACE 42 RBRACE 43 SEMI 44 
%token INTEGER_LITERAL 45 STRING_LITERAL 46 FLOAT_LITERAL 47
%token BOOL_LITERAL 48 IDENTIFIER 49

%% /* Grammar rules and actions follow. */

exp:
  IF_KWD
;

%%