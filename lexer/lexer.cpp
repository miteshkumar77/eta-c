#include "lexer.hpp"

#include <string>

namespace minic::lexer
{
    namespace
    {
        char const *const none_if_null(char const *const maybe_null);
    }

    bool StateHandle::operator==(const StateHandle &other) const
    {
        return (this->error_msg == nullptr && other.error_msg == nullptr) ||
               ((this->error_msg != nullptr && other.error_msg != nullptr) &&
                std::string(this->error_msg) == std::string(other.error_msg));
    }

    bool token::operator==(const token &other) const
    {
        return (this->tag == other.tag) &&
               (this->meta == other.meta);
    }

    LoggerT &operator<<(LoggerT &logger, const StateHandle &state)
    {
        return logger << "StateHandle{ .error_msg=" << none_if_null(state.error_msg) << " }";
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
        case MC_OR_BIN:
            return "MC_OR";
        case MC_LPAR:
            return "MC_LPAR";
        case MC_RPAR:
            return "MC_RPAR";
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
        case MC_SEMI:
            return "MC_SEMI";
            // no-default
        }
        return "~~UNKNOWN~~";
    }

    namespace
    {
        char const *const none_if_null(char const *const maybe_null)
        {
            if (maybe_null)
            {
                return maybe_null;
            }
            else
            {
                return "[none]";
            }
        }
    }
}