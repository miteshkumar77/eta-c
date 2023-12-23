%{
#include "lexer/utils.hpp"
#include "lexer/lexer.hpp" // define all static state here

/* Max size of string constants */
#define MAX_STR_CONST 1025
#define YY_NO_UNPUT   /* keep g++ happy */

char string_buf[MAX_STR_CONST]; /* to assemble string constants */
char *string_buf_ptr;

extern YYSTYPE minic_state_handle;

size_t comment_depth = 0;
%}


WHITESPACE      (" "|\f|\r|\t|\v)

%%

{WHITESPACE} { }
.            { minic_state_handle.error_msg = yytext; return MC_ERROR; }

%%