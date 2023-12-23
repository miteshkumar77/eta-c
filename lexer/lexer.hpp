#pragma once
struct MinicStateHandle
{
    char *error_msg{nullptr};
};

#define YYSTYPE MinicStateHandle

YYSTYPE minic_state_handle;

enum minic_tokentype
{
    ERROR = 11,
    WHITESPACE = 12,
};

#define YYTOKENTYPE minic_tokentype

#define MC_ERROR minic_tokentype::ERROR 
#define MC_WHITESPACE minic_tokentype::WHITESPACE