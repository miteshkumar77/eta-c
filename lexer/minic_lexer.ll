%{
#include "lexer/utils.hpp"
#include "lexer/lexer.hpp" // define all static state here

/* Max size of string constants */
#define MAX_STR_CONST 1025
#define YY_NO_UNPUT   /* keep g++ happy */

#define YY_USER_ACTION \
    minic::lexer::rule_setup();

char string_buf[MAX_STR_CONST]; /* to assemble string constants */
char *string_buf_ptr = nullptr;

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
LBRACE             "{"
RBRACE             "}"
SEMI               ";"

LINE_CMT           "//"
SCOPE_CMT_BEGIN    "/*"
SCOPE_CMT_END      "*/"

WHITESPACE      (" "|\f|\r|\t|\v)

DIGIT           [0-9]

INTEGER_LITERAL {DIGIT}+
FLOAT_LITERAL   {DIGIT}+\.{DIGIT}*
TRUE_BOOL_LITERAL    "true"
FALSE_BOOL_LITERAL   "false"

STR_TERM          "\""

%x              LINE_CMT_SCOPE
%x              SCOPE_CMT_SCOPE
%x              STRING_SCOPE

%%

{WHITESPACE}   { }
{IF_KWD}       { return MC_IF_KWD; }
{ELSE_KWD}     { return MC_ELSE_KWD; }
{WHILE_KWD}    { return MC_WHILE_KWD; }
{BREAK_KWD}    { return MC_BREAK_KWD; }
{CONTINUE_KWD} { return MC_CONTINUE_KWD; }

{EXIT_KWD}     { return MC_EXIT_KWD; }
{PRINT_KWD}    { return MC_PRINT_KWD; }
{PRINTLN_KWD}  { return MC_PRINTLN_KWD; }

{INT_TYPE}     { return MC_INT_TYPE; }
{DOUBLE_TYPE}  { return MC_DOUBLE_TYPE; }
{STRING_TYPE}  { return MC_STRING_TYPE; }
{BOOL_TYPE}    { return MC_BOOL_TYPE; }


{MUL_BIN}      { return MC_MUL_BIN; }
{DIV_BIN}      { return MC_DIV_BIN; }   
{PLUS_BIN}     { return MC_PLUS_BIN; } 
{MINUS_BIN}    { return MC_MINUS_BIN; }  
{MOD_BIN}      { return MC_MOD_BIN; }
{LT_BIN}       { return MC_LT_BIN; }
{GT_BIN}       { return MC_GT_BIN; }
{LTEQ_BIN}     { return MC_LTEQ_BIN; } 
{GTEQ_BIN}     { return MC_GTEQ_BIN; } 
{ASSIGN_BIN}   { return MC_ASSIGN_BIN; }   
{EQ_BIN}       { return MC_EQ_BIN; }
{NOTEQ_BIN}    { return MC_NOTEQ_BIN; }  
{NOT_UN}       { return MC_NOT_UN; }
{AND_BIN}      { return MC_AND_BIN; }
{LPAR}         { return MC_LPAR; }
{RPAR}         { return MC_RPAR; }
{LBRACE}       { return MC_LBRACE; }
{RBRACE}       { return MC_RBRACE; }
{OR_BIN}       { return MC_OR_BIN; }
{SEMI}         { return MC_SEMI; }

{INTEGER_LITERAL}  { 
    constexpr minic::lexer::token_tag rt = MC_INTEGER_LITERAL;
    minic::lexer::minic_state_handle.get_writer<rt>(
        minic::lexer::ArbitraryLiteralMeta{.content=yytext}
    );
    return rt;
}

{FLOAT_LITERAL}  { 
    constexpr minic::lexer::token_tag rt = MC_FLOAT_LITERAL;
    minic::lexer::minic_state_handle.get_writer<rt>(
        minic::lexer::ArbitraryLiteralMeta{.content=yytext}
    );
    return rt;
}

{TRUE_BOOL_LITERAL}  {
    constexpr minic::lexer::token_tag rt = MC_BOOL_LITERAL;
    minic::lexer::minic_state_handle.get_writer<rt>(
        minic::lexer::BoolLiteralMeta{.value=true}
    );
    return rt;
}

{FALSE_BOOL_LITERAL}  {
    constexpr minic::lexer::token_tag rt = MC_BOOL_LITERAL;
    minic::lexer::minic_state_handle.get_writer<rt>(
        minic::lexer::BoolLiteralMeta{.value=false}
    );
    return rt;
}

{LINE_CMT}     {
    BEGIN(LINE_CMT_SCOPE);
}
<LINE_CMT_SCOPE>\n {
    BEGIN(INITIAL);
}
{SCOPE_CMT_BEGIN} {
    BEGIN(SCOPE_CMT_SCOPE);
}
<SCOPE_CMT_SCOPE>{SCOPE_CMT_END} {
    BEGIN(INITIAL);
}
{SCOPE_CMT_END} {
    minic::lexer::minic_state_handle.get_writer<MC_ERROR>(minic::lexer::ErrorMeta{.error_msg="unmatched end comment"});
    return MC_ERROR;
}
{STR_TERM}               {
    string_buf_ptr = string_buf;
    BEGIN(STRING_SCOPE);
}
<STRING_SCOPE>"\n"       {
    minic::lexer::minic_state_handle.get_writer<MC_ERROR>(minic::lexer::ErrorMeta{.error_msg="multi-line string not allowed"});
    return MC_ERROR;
}
<STRING_SCOPE>{STR_TERM} {
    BEGIN(INITIAL);
    constexpr minic::lexer::token_tag rt = MC_STRING_LITERAL;
    minic::lexer::minic_state_handle.get_writer<rt>(
        minic::lexer::ArbitraryLiteralMeta{.content=std::string(string_buf, string_buf_ptr - string_buf)}
    );
    string_buf_ptr = nullptr;
    return rt;
}
<STRING_SCOPE>"\\". {
    if ((string_buf_ptr - string_buf + 2) > MAX_STR_CONST) {
        minic::lexer::minic_state_handle.get_writer<MC_ERROR>(minic::lexer::ErrorMeta{.error_msg="string constant too long"});
        BEGIN(INITIAL);
        return MC_ERROR;
    }

    const char& specialChar = yytext[1];
    switch (specialChar)
    {
    case 'b':
        (*string_buf_ptr++) = '\b';
        break;
    case 't':
        (*string_buf_ptr++) = '\t';
        break;
    case 'n':
        (*string_buf_ptr++) = '\n';
        break;
    case 'f':
        (*string_buf_ptr++) = '\f';
        break;
    default:
        (*string_buf_ptr++) = specialChar;
    }
}
<STRING_SCOPE>.         {
    if ((string_buf_ptr - string_buf + 2) > MAX_STR_CONST) {
        minic::lexer::minic_state_handle.get_writer<MC_ERROR>(minic::lexer::ErrorMeta{.error_msg="string constant too long"});
        BEGIN(INITIAL);
        return MC_ERROR;
    }
    *(string_buf_ptr++) = yytext[0];
}


<<EOF>>        { return 0; }

.              { 
    minic::lexer::minic_state_handle.get_writer<MC_ERROR>(minic::lexer::ErrorMeta{.error_msg=yytext});
    return MC_ERROR; 
}

%%