#pragma once

#include <variant>

#include "base/logger.hpp"
#include "lexer/lexer.hpp"
#include <FlexLexer.h>

namespace minic::parser {

  class bisonFlexLexer : public yyFlexLexer
  {
  public:
    int yylakes(minic::lexer::StateHandle*)
    {
      return this->yylex();
    }
  };

  #define YYSTYPE minic::lexer::StateHandle
}