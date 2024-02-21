#include "base/defs.hpp"

namespace eta {
LoggerT &operator<<(LoggerT &logger, const TokenMeta &tokenMeta) {
  const std::wstring tmpName(tokenMeta.identifier_name.begin(),
                             tokenMeta.identifier_name.end());
  const std::wstring tmpError(tokenMeta.error.begin(), tokenMeta.error.end());
  return logger << "TokenMeta{.line_number=" << tokenMeta.line_number
                << ", .column_number=" << tokenMeta.column_number
                << ", .uc_content=" << tokenMeta.uc_content
                << ", .identifier_name=" << tmpName
                << ", .int_content=" << tokenMeta.int_content
                << ", .error=" << tmpError << "}";
}

bool operator==(const TokenMeta &lhs, const TokenMeta &rhs) {
  return lhs.uc_content == rhs.uc_content &&
         lhs.identifier_name == rhs.identifier_name &&
         lhs.int_content == rhs.int_content && lhs.error == rhs.error;
}

void TokenMeta::prime_before_begin_lexing() {
  this->line_number = -1;
  this->column_number = -1;
}

bool TokenMeta::is_for_first_token() {
  return this->line_number == -1 && this->column_number == -1;
}

void TokenMeta::prime_for_next_token(int64_t line_number,
                                     int64_t column_number) {
  (*this) =
      TokenMeta{.line_number = line_number, .column_number = column_number};
}
} // namespace eta