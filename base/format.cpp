#include "format.hpp"

#include <algorithm>
#include <charconv>
#include <cstring>
#include <iostream>
#include <string_view>

namespace eta {

namespace util {
std::wstring to_wstring(char const *const chars) {
  const std::string_view iter(chars);
  return std::wstring(iter.begin(), iter.end());
}
} // namespace util

auto int_util::from_str(char const *const int_str)
    -> std::variant<int64_t, err> {
  int64_t result{};
  char const *const end = std::strlen(int_str) + int_str;
  auto [ptr, ec] = std::from_chars(int_str, end, result);
  if (ec == std::errc{}) {
    if (ptr != end) {
      return err{};
    }
    return result;
  } else {
    return err{};
  }
}

void ustr_util::append_ascii_chars(std::wstring &wstr,
                                   char const *const chars) {

  std::string_view chars_iterable(chars);
  for (char c : chars_iterable) {
    wstr.push_back(static_cast<wchar_t>(c));
  }
}

auto ustr_util::append_unicode_char(std::wstring &wstr,
                                    char const *const unicode_char)
    -> std::optional<err> {

  /**
   * unicode_char = \x{...}
   */
  constexpr static uint32_t max_ucode{0x10FFFF};
  uint32_t result{};
  char const *const begin = unicode_char + 3;
  char const *const end = std::strlen(begin) - 1 + begin;
  for (char const *c = begin; c != end; ++c) {
    std::cout << "decoding char=" << *c << std::endl;
  }
  auto [ptr, ec] = std::from_chars(begin, end, result, 16);
  if (ec == std::errc{}) {
    if (ptr != end) {
      return err{};
    }
    if (result > max_ucode) {
      return err{};
    }
    std::cout << "final result=" << result << std::endl;
    wstr.push_back(static_cast<wchar_t>(result));
    return std::nullopt;
  } else {
    return err{};
  }
}

} // namespace eta