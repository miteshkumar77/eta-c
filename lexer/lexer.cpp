#include "lexer.hpp"

#include <string>

namespace
{
    char const *const none_if_null(char const *const maybe_null);
}

bool MinicStateHandle::operator==(const MinicStateHandle &other) const
{
    return (this->error_msg == nullptr && other.error_msg == nullptr) ||
           ((this->error_msg != nullptr && other.error_msg != nullptr) &&
            std::string(this->error_msg) == std::string(other.error_msg));
}

bool token::operator==(const token &other) const
{
    return (this->token_type == other.token_type) &&
           (this->token_meta == other.token_meta);
}

LoggerT &operator<<(LoggerT &logger, const MinicStateHandle &state)
{
    return logger << "MinicStateHandle{ .error_msg=" << none_if_null(state.error_msg) << " }";
}

LoggerT &operator<<(LoggerT &logger, const token &t)
{
    return logger << "token{ .token_type=" << token_type_to_string(t.token_type)
                  << ", .token_meta=" << t.token_meta << " }";
}

char const *const token_type_to_string(minic_tokentype tt)
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