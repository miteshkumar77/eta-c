#include "parser/minic_parser.h"
#include <fstream>
#include "base/ast.hpp"

int main(int argc, char** argv)
{
  const auto _= minic::ast::BaseNodeArgs{};
  minic::ast::Assignment assignment(
    _,
    minic::ast::TypeId(_, minic::ast::basic_type::bool_type),
    minic::ast::Identifier(_, "i"),
    minic::ast::Expression(_,
      minic::ast::basic_type::bool_type, minic::ast::BooleanLiteralExpression(
        _, false
      ))
  );
  std::cout << assignment << std::endl;
  // if (argc != 2) {
  //   std::cerr << "ERROR: usage: ./main <path_to_lexer_input>" << std::endl;
  //   return 0;
  // }
  // std::ifstream fs;
  // fs.open(argv[1], std::ifstream::in);
  // minic::parser::bisonFlexLexer fl;
  // yy::parser p(fs, std::cerr, fl);
  // int rc { 0 };
  // while((rc = p.parse()) != static_cast<int>(0))
  // {
  //   std::cout << rc << std::endl;
  // }
}