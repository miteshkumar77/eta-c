#include "lexer/lexer_defs.hpp"
#include "base/defs.hpp"

#include <cassert>

namespace eta {
etaFlexLexer::etaFlexLexer(std::istream *arg_yyin, std::ostream *arg_yyout)
    : base_t{arg_yyin, arg_yyout} {
  symbol_to_parser.prime_before_begin_lexing();
  assert(symbol_to_parser.is_for_first_token());
}

void etaFlexLexer::LexerOutput(char const *const yytext, int yyleng) {
  yyout << "line_num: " << symbol_to_parser.line_number
        << ": col_num: " << symbol_to_parser.column_number << ": ";
  base_t::LexerOutput(yytext, yyleng);
}
} // namespace eta