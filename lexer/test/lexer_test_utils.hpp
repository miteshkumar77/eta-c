#pragma once
#include "base/tokens.hpp"
#include <string_view>
#include <vector>

namespace minic::lexer::test {
std::vector<token> tokenize(const std::string_view input);
}