#include "ast.hpp"
#include "defs.hpp"
#include <iostream>
int main() {
  std::wstring ws;
  auto ec = eta::ustr_util::append_unicode_char(ws, "\\x{3a}");
  if (ec.has_value()) {
    std::wcout << "ERROR" << std::endl;
  }
  std::wcout << ws << std::endl;
}