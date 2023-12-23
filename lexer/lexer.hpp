#pragma once

#include <ostream>

using LoggerT = std::ostream;

struct MinicStateHandle
{
    char *error_msg{nullptr};
    bool operator==(const MinicStateHandle& other) const;
};

LoggerT& operator<<(LoggerT& logger, const MinicStateHandle& state);

#define YYSTYPE MinicStateHandle

YYSTYPE minic_state_handle;

enum minic_tokentype
{
    ERROR = 11,
    WHITESPACE = 12,
    IF_KWD = 13,
};

#define YYTOKENTYPE minic_tokentype

#define MC_ERROR minic_tokentype::ERROR 
#define MC_WHITESPACE minic_tokentype::WHITESPACE
#define MC_IF_KWD minic_tokentype::IF_KWD

struct token {
    minic_tokentype token_type;
    MinicStateHandle token_meta;
    bool operator==(const token& other) const;
};

LoggerT& operator<<(LoggerT& logger, const token& token);
char const*const token_type_to_string(minic_tokentype tt);