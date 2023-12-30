#include "parser/minic_parser.h"
#include <fstream>
#include "base/ast.hpp"

int main(int argc, char **argv)
{
  using namespace minic::ast;
  using bna = BaseNodeArgs;
  const auto _ = bna{};
  Statement stmt1(bna{.new_line = true, .indent = 2},
    Declaration(
      _,
      TypeId(bna{.new_line = true, .indent = 3}, basic_type::int_type),
      Identifier(bna{.new_line = true, .indent = 3}, "i"),
      Expression(bna{.new_line = true, .indent = 3}, basic_type::int_type, ArbitraryLiteralExpression(_, "123"))));

  Statement stmt2(bna{.new_line = true, .indent = 2},
    Assignment(
      _,
      Identifier(bna{.new_line = true, .indent = 3}, "i"),
      Expression(bna{.new_line = true, .indent = 3}, basic_type::int_type, ArbitraryLiteralExpression(_, "125"))));

  Block blk1(_, Block::stmt_list_t{
    stmt1, stmt2
  });

  std::cout << blk1 << std::endl;
  // const auto _= minic::ast::BaseNodeArgs{};
  // minic::ast::Statement stmt(_, minic::ast::Declaration({.}, minic::ast::TypeId(_, minic::ast::basic_type::int_type),
  // minic::ast::Identifier(_, "i"), minic::ast::Expression(_, minic::ast::basic_type::int_type, minic::ast::ArbitraryLiteralExpression(_, "123"))));
  // minic::ast::Statement block_stmt(_, minic::ast::Block(_, minic::ast::Block::stmt_list_t{stmt, stmt, stmt}));
  // std::cout << block_stmt << std::endl;
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