#include "lexer/test/test_lexer.hpp"
#include "parser/lib_eta_parser_utils.hpp"

#include <fstream>
#include <stdexcept>

namespace eta {

extern TokenMeta symbol_to_parser;

namespace test {

LoggerT &operator<<(LoggerT &logger, const LexerRval &rv) {
  return logger << "LexerRval{.type=" << token_to_string(rv.type)
                << ", meta=" << rv.meta << "}";
}

std::vector<LexerRval> tokenize_istream(std::istream &is) {
  std::vector<LexerRval> result;
  etaFlexLexer fl(&is, &std::cerr);
  int rc{0};
  while ((rc = fl.yylex()) != static_cast<int>(0)) {
    result.emplace_back(LexerRval{
        .type = static_cast<yy::parser::token::yytokentype>(rc),
        .meta = std::move(symbol_to_parser),
    });
  }
  return result;
}

std::vector<LexerRval> tokenize_file(const std::string &fpath) {
  std::ifstream ifs(fpath, std::ios::in);
  if (!ifs.good()) {
    std::cerr << "ERROR: eta::test::tokenize_file: coult not open the file!"
              << std::endl;
    throw std::runtime_error("failed to open file " + fpath);
  }
  return tokenize_istream(ifs);
}

bool operator==(const LexerRval &lhs, const LexerRval &rhs) {
  return (lhs.type == rhs.type) && (lhs.meta == rhs.meta);
}

} // namespace test
} // namespace eta