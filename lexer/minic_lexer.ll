%{
#include "lexer/utils.hpp"
#include "lexer/lexer.hpp" // define all static state here

/* Max size of string constants */
#define MAX_STR_CONST 1025
#define YY_NO_UNPUT   /* keep g++ happy */

char string_buf[MAX_STR_CONST]; /* to assemble string constants */
char *string_buf_ptr;

extern YYSTYPE minic::lexer::minic_state_handle;

size_t comment_depth = 0;
%}

IF_KWD          if
ELSE_KWD        else
WHILE_KWD       while
BREAK_KWD       break
CONTINUE_KWD    continue

EXIT_KWD        exit
PRINT_KWD       print
PRINTLN_KWD     println

INT_TYPE        int
DOUBLE_TYPE     double
STRING_TYPE     string
BOOL_TYPE       bool

MUL_BIN            "*"
DIV_BIN            "/"
PLUS_BIN           "+"
MINUS_BIN          "-"
MOD_BIN            "%"
LT_BIN             "<"
GT_BIN             ">"
LTEQ_BIN           "<="
GTEQ_BIN           ">="
ASSIGN_BIN         "="
EQ_BIN             "=="
NOTEQ_BIN          "!="
NOT_UN             "!"
AND_BIN            "&&"
OR_BIN             "||"
LPAR               "("
RPAR               ")"
SEMI               ";"

WHITESPACE      (" "|\f|\r|\t|\v)

BLOCK_COMMENT : 


%%

{WHITESPACE}   { }
{IF_KWD}       { minic::lexer::minic_state_handle.error_msg = nullptr; return MC_IF_KWD; }
{ELSE_KWD}     { minic::lexer::minic_state_handle.error_msg = nullptr; return MC_ELSE_KWD; }
{WHILE_KWD}    { minic::lexer::minic_state_handle.error_msg = nullptr; return MC_WHILE_KWD; }
{BREAK_KWD}    { minic::lexer::minic_state_handle.error_msg = nullptr; return MC_BREAK_KWD; }
{CONTINUE_KWD} { minic::lexer::minic_state_handle.error_msg = nullptr; return MC_CONTINUE_KWD; }

{EXIT_KWD}     { minic::lexer::minic_state_handle.error_msg = nullptr; return MC_EXIT_KWD; }
{PRINT_KWD}    { minic::lexer::minic_state_handle.error_msg = nullptr; return MC_PRINT_KWD; }
{PRINTLN_KWD}  { minic::lexer::minic_state_handle.error_msg = nullptr; return MC_PRINTLN_KWD; }

{INT_TYPE}     { minic::lexer::minic_state_handle.error_msg = nullptr; return MC_INT_TYPE; }
{DOUBLE_TYPE}  { minic::lexer::minic_state_handle.error_msg = nullptr; return MC_DOUBLE_TYPE; }
{STRING_TYPE}  { minic::lexer::minic_state_handle.error_msg = nullptr; return MC_STRING_TYPE; }
{BOOL_TYPE}    { minic::lexer::minic_state_handle.error_msg = nullptr; return MC_BOOL_TYPE; }


{MUL_BIN}      { minic::lexer::minic_state_handle.error_msg = nullptr; return MC_MUL_BIN; }
{DIV_BIN}      { minic::lexer::minic_state_handle.error_msg = nullptr; return MC_DIV_BIN; }   
{PLUS_BIN}     { minic::lexer::minic_state_handle.error_msg = nullptr; return MC_PLUS_BIN; } 
{MINUS_BIN}    { minic::lexer::minic_state_handle.error_msg = nullptr; return MC_MINUS_BIN; }  
{MOD_BIN}      { minic::lexer::minic_state_handle.error_msg = nullptr; return MC_MOD_BIN; }
{LT_BIN}       { minic::lexer::minic_state_handle.error_msg = nullptr; return MC_LT_BIN; }
{GT_BIN}       { minic::lexer::minic_state_handle.error_msg = nullptr; return MC_GT_BIN; }
{LTEQ_BIN}     { minic::lexer::minic_state_handle.error_msg = nullptr; return MC_LTEQ_BIN; } 
{GTEQ_BIN}     { minic::lexer::minic_state_handle.error_msg = nullptr; return MC_GTEQ_BIN; } 
{ASSIGN_BIN}   { minic::lexer::minic_state_handle.error_msg = nullptr; return MC_ASSIGN_BIN; }   
{EQ_BIN}       { minic::lexer::minic_state_handle.error_msg = nullptr; return MC_EQ_BIN; }
{NOTEQ_BIN}    { minic::lexer::minic_state_handle.error_msg = nullptr; return MC_NOTEQ_BIN; }  
{NOT_UN}       { minic::lexer::minic_state_handle.error_msg = nullptr; return MC_NOT_UN; }
{AND_BIN}      { minic::lexer::minic_state_handle.error_msg = nullptr; return MC_AND_BIN; }
{LPAR}         { minic::lexer::minic_state_handle.error_msg = nullptr; return MC_LPAR; }
{RPAR}         { minic::lexer::minic_state_handle.error_msg = nullptr; return MC_RPAR; }
{OR_BIN}       { minic::lexer::minic_state_handle.error_msg = nullptr; return MC_OR_BIN; }
{SEMI}         { minic::lexer::minic_state_handle.error_msg = nullptr; return MC_SEMI; }

.              { minic::lexer::minic_state_handle.error_msg = yytext; return MC_ERROR; }

%%