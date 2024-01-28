#pragma once
#include "base/logger.hpp"

#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace eta {

namespace util {
std::wstring to_wstring(char const *const chars);
} // namespace util

struct int_util {
  struct err {};
  static std::variant<int64_t, err> from_str(char const *const int_str);
};

struct ustr_util {
  struct err {};
  static void append_ascii_chars(std::wstring &wstr, char const *const chars);
  static std::optional<err> append_unicode_char(std::wstring &wstr,
                                                char const *const unicode_char);
};

} // namespace eta