#include "lexer.hpp"

#include <string>

namespace minic::lexer
{
    bool DefaultMeta::operator==(const DefaultMeta&) const
    {
        return true;
    }

    LoggerT& operator<<(LoggerT& logger, const DefaultMeta& defaultMeta)
    {
        return logger << "DefaultMeta{}";
    }

    bool ErrorMeta::operator==(const ErrorMeta& other) const
    {
        return this->error_msg == other.error_msg;
    }

    LoggerT& operator<<(LoggerT& logger, const ErrorMeta& errorMeta)
    {
        return logger << "ErrorMeta{ .error_msg=" << errorMeta.error_msg << " }";
    }

    bool ArbitraryLiteralMeta::operator==(const ArbitraryLiteralMeta& other) const
    {
        return this->content == other.content;
    }

    LoggerT& operator<<(LoggerT& logger, const ArbitraryLiteralMeta& ArbitraryLiteralMeta)
    {
        return logger << "ArbitraryLiteralMeta{ .content=" << ArbitraryLiteralMeta.content << " }";
    }

    bool BoolLiteralMeta::operator==(const BoolLiteralMeta& other) const
    {
        return this->value == other.value;
    }

    LoggerT& operator<<(LoggerT& logger, const BoolLiteralMeta& boolLiteralMeta)
    {
        return logger << "BoolLiteralMeta{ .value=" << std::boolalpha << boolLiteralMeta.value << " }";
    }

    bool StateHandle::operator==(const StateHandle &other) const
    {
        return this->val == other.val;
    }

    bool token::operator==(const token &other) const
    {
        return (this->tag == other.tag) &&
               (this->meta == other.meta);
    }

    LoggerT &operator<<(LoggerT &logger, const StateHandle &state)
    {
        logger << "StateHandle{ .val=";
        std::visit([&logger](auto&& arg) {
            logger << arg;
        }, state.val);
        return logger << " }";
    }

    LoggerT &operator<<(LoggerT &logger, const token &t)
    {
        return logger << "token{ .tag=" << token_tag_to_string(t.tag)
                      << ", .meta=" << t.meta << " }";
    }

    char const *const token_tag_to_string(token_tag tt)
    {
        switch (tt)
        {
        case MC_ERROR:
            return "MC_ERROR";
        case MC_WHITESPACE:
            return "MC_WHITESPACE";
        case MC_IF_KWD:
            return "MC_IF_KWD";
        case MC_ELSE_KWD:
            return "MC_ELSE_KWD";
        case MC_WHILE_KWD:
            return "MC_WHILE_KWD";
        case MC_BREAK_KWD:
            return "MC_BREAK_KWD";
        case MC_CONTINUE_KWD:
            return "MC_CONTINUE_KWD";
        case MC_EXIT_KWD:
            return "MC_EXIT_KWD";
        case MC_PRINT_KWD:
            return "MC_PRINT_KWD";
        case MC_PRINTLN_KWD:
            return "MC_PRINTLN_KWD";
        case MC_INT_TYPE:
            return "MC_INT_TYPE";
        case MC_DOUBLE_TYPE:
            return "MC_DOUBLE_TYPE";
        case MC_STRING_TYPE:
            return "MC_STRING_TYPE";
        case MC_BOOL_TYPE:
            return "MC_BOOL_TYPE";
        case MC_MUL_BIN:
            return "MC_MUL_BIN";
        case MC_DIV_BIN:
            return "MC_DIV_BIN";
        case MC_PLUS_BIN:
            return "MC_PLUS_BIN";
        case MC_MINUS_BIN:
            return "MC_MINUS_BIN";
        case MC_MOD_BIN:
            return "MC_MOD_BIN";
        case MC_LT_BIN:
            return "MC_LT_BIN";
        case MC_GT_BIN:
            return "MC_GT_BIN";
        case MC_LTEQ_BIN:
            return "MC_LTEQ_BIN";
        case MC_GTEQ_BIN:
            return "MC_GTEQ_BIN";
        case MC_ASSIGN_BIN:
            return "MC_ASSIGN_BIN";
        case MC_EQ_BIN:
            return "MC_EQ_BIN";
        case MC_NOTEQ_BIN:
            return "MC_NOTEQ_BIN";
        case MC_NOT_UN:
            return "MC_NOT_UN";
        case MC_AND_BIN:
            return "MC_AND_BIN";
        case MC_OR_BIN:
            return "MC_OR";
        case MC_LPAR:
            return "MC_LPAR";
        case MC_RPAR:
            return "MC_RPAR";
        case MC_LBRACE:
            return "MC_LBRACE";
        case MC_RBRACE:
            return "MC_RBRACE";
        case MC_SEMI:
            return "MC_SEMI";
        case MC_INTEGER_LITERAL:
            return "MC_INTEGER_LITERAL";
        case MC_STRING_LITERAL:
            return "MC_STRING_LITERAL";
        case MC_FLOAT_LITERAL:
            return "MC_FLOAT_LITERAL";
        case MC_BOOL_LITERAL:
            return "MC_BOOL_LITERAL";
            // no-default
        }
        return "~~UNKNOWN~~";
    }

    void rule_setup()
    {
        minic_state_handle = StateHandle{};
    }
}