#include "base/tokens.hpp"
#include <FlexLexer.h>
#include <iostream>
#include <fstream>

int main(int argc, char **argv)
{
  if (argc != 2) {
    std::cerr << "ERROR: usage: ./main <path_to_lexer_input>" << std::endl;
    return 0;
  }
  std::ifstream fs;
  fs.open(argv[1], std::ifstream::in);
  yyFlexLexer fl(&fs, &std::cerr);
  int rc { 0 };
  while ((rc = fl.yylex()) != static_cast<int>(0))
  {
    std::cout << minic::lexer::token{
      .tag=static_cast<minic::lexer::token_tag>(rc),
      .meta=minic::lexer::minic_state_handle,
    } << std::endl;
  }

}