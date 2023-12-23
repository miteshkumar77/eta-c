#pragma once

#include "base/logger.hpp"
namespace minic::lexer {
struct StateHandle
{
    char *error_msg{nullptr};
    bool operator==(const StateHandle& other) const;
};

LoggerT& operator<<(LoggerT& logger, const StateHandle& state);

#define YYSTYPE minic::lexer::StateHandle

YYSTYPE minic_state_handle;

enum token_tag
{
    ERROR = 11,
    WHITESPACE = 12,
    IF_KWD = 13,
};

#define YYTOKENTYPE minic::lexer::token_tag

#define MC_ERROR minic::lexer::token_tag::ERROR 
#define MC_WHITESPACE minic::lexer::token_tag::WHITESPACE
#define MC_IF_KWD minic::lexer::token_tag::IF_KWD

struct token {
    token_tag tag;
    StateHandle meta;
    bool operator==(const token& other) const;
};

LoggerT& operator<<(LoggerT& logger, const token& token);
char const*const token_tag_to_string(token_tag tt);
}