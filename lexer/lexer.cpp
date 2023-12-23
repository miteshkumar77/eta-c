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