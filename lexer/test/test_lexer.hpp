#pragma once

#include "base/defs.hpp"
#include "base/logger.hpp"
#include "parser/lib_eta_parser.h"
#include <iostream>
#include <vector>

namespace eta::test {

struct LexerRval {
  yy::parser::token::yytokentype type;
  TokenMeta meta;
};

bool operator==(const LexerRval &lhs, const LexerRval &rhs);

std::vector<LexerRval> tokenize_istream(std::istream &is);
std::vector<LexerRval> tokenize_file(const std::string &fpath);

LoggerT &operator<<(LoggerT &logger, const LexerRval &rv);

} // namespace eta::test