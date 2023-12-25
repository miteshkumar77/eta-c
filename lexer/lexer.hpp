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

inline YYSTYPE minic_state_handle {};

void rule_setup();

enum token_tag
{
    ERROR = 11,
    WHITESPACE = 12,
    IF_KWD = 13,
    ELSE_KWD = 14,
    WHILE_KWD = 15,
    BREAK_KWD = 16,
    CONTINUE_KWD = 17,

    EXIT_KWD = 18,
    PRINT_KWD = 19,
    PRINTLN_KWD = 20,

    INT_TYPE = 21,
    DOUBLE_TYPE = 22,
    STRING_TYPE = 23,
    BOOL_TYPE = 24,

    MUL_BIN = 25,
    DIV_BIN = 26,
    PLUS_BIN = 27,
    MINUS_BIN = 28,
    MOD_BIN = 29,
    LT_BIN = 30,
    GT_BIN = 31,
    LTEQ_BIN = 32,
    GTEQ_BIN = 33,
    ASSIGN_BIN = 34,
    EQ_BIN = 35,
    NOTEQ_BIN = 36,
    NOT_UN = 37,
    AND_BIN = 38,
    OR_BIN = 39,

    LPAR   = 40,
    RPAR   = 41,
    SEMI   = 42,

    INTEGER_LITERAL = 43,
};

#define YYTOKENTYPE minic::lexer::token_tag

#define MC_ERROR minic::lexer::token_tag::ERROR 
#define MC_WHITESPACE minic::lexer::token_tag::WHITESPACE
#define MC_IF_KWD minic::lexer::token_tag::IF_KWD
#define MC_ELSE_KWD minic::lexer::token_tag::ELSE_KWD
#define MC_WHILE_KWD minic::lexer::token_tag::WHILE_KWD
#define MC_BREAK_KWD minic::lexer::token_tag::BREAK_KWD
#define MC_CONTINUE_KWD minic::lexer::token_tag::CONTINUE_KWD
#define MC_EXIT_KWD minic::lexer::token_tag::EXIT_KWD
#define MC_PRINT_KWD minic::lexer::token_tag::PRINT_KWD
#define MC_PRINTLN_KWD minic::lexer::token_tag::PRINTLN_KWD
#define MC_INT_TYPE minic::lexer::token_tag::INT_TYPE
#define MC_DOUBLE_TYPE minic::lexer::token_tag::DOUBLE_TYPE
#define MC_STRING_TYPE minic::lexer::token_tag::STRING_TYPE
#define MC_BOOL_TYPE minic::lexer::token_tag::BOOL_TYPE
#define MC_MUL_BIN minic::lexer::token_tag::MUL_BIN
#define MC_DIV_BIN minic::lexer::token_tag::DIV_BIN
#define MC_PLUS_BIN minic::lexer::token_tag::PLUS_BIN
#define MC_MINUS_BIN minic::lexer::token_tag::MINUS_BIN
#define MC_MOD_BIN minic::lexer::token_tag::MOD_BIN
#define MC_LT_BIN minic::lexer::token_tag::LT_BIN
#define MC_GT_BIN minic::lexer::token_tag::GT_BIN
#define MC_LTEQ_BIN minic::lexer::token_tag::LTEQ_BIN
#define MC_GTEQ_BIN minic::lexer::token_tag::GTEQ_BIN
#define MC_ASSIGN_BIN minic::lexer::token_tag::ASSIGN_BIN
#define MC_EQ_BIN minic::lexer::token_tag::EQ_BIN
#define MC_NOTEQ_BIN minic::lexer::token_tag::NOTEQ_BIN
#define MC_NOT_UN minic::lexer::token_tag::NOT_UN
#define MC_AND_BIN minic::lexer::token_tag::AND_BIN
#define MC_OR_BIN minic::lexer::token_tag::OR_BIN
#define MC_LPAR minic::lexer::token_tag::LPAR
#define MC_RPAR minic::lexer::token_tag::RPAR
#define MC_SEMI minic::lexer::token_tag::SEMI
#define MC_INTEGER_LITERAL minic::lexer::token_tag::INTEGER_LITERAL

struct token {
    token_tag tag;
    StateHandle meta;
    bool operator==(const token& other) const;
};

LoggerT& operator<<(LoggerT& logger, const token& token);
char const*const token_tag_to_string(token_tag tt);
}