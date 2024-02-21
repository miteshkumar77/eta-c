#pragma once

#if !defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

namespace eta {

// ONLY ONE CAN EXIST CONSTRUCTED AT ANY GIVEN TIME
class etaFlexLexer : public yyFlexLexer {
  using base_t = yyFlexLexer;
  using base_t::LexerOutput;

  void LexerOutput(char const *const yytext, int yyleng);

public:
  etaFlexLexer(std::istream *arg_yyin, std::ostream *arg_yyout);
};
} // namespace eta