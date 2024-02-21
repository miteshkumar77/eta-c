#pragma once

#include "base/ast.hpp"
#include "base/format.hpp"
#include "base/logger.hpp"

int64_t eta_line_number();
int64_t eta_column_number();

namespace eta {

struct TokenMeta {
  int64_t line_number{0};
  int64_t column_number{0};

  std::wstring uc_content;
  std::string identifier_name;
  int64_t int_content{0};
  std::string error;
  void prime_before_begin_lexing();
  bool is_for_first_token();
  void prime_for_next_token(int64_t line_number, int64_t column_number);
};
bool operator==(const TokenMeta &lhs, const TokenMeta &rhs);

LoggerT &operator<<(LoggerT &logger, const TokenMeta &tokenMeta);

inline TokenMeta symbol_to_parser;
} // namespace eta