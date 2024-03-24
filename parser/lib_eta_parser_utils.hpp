#pragma once

#include <memory>

#include "lexer/lexer_defs.hpp"
#include "parser/lib_eta_parser.h"

namespace eta {
char const *token_to_string(yy::parser::token::yytokentype token);

struct ParserDriver {
  ParserDriver(std::istream &yyin, std::ostream &yyout);

  template <typename AstNodeT, typename... ArgsT> AstNodeT &create();

  std::reference_wrapper<ast::ProgramAstNode> m_program;

  etaFlexLexer m_lexer;
  yy::parser m_parser;

private:
  std::vector<std::unique_ptr<ast::BaseAstNode>> m_node_deleter;
};

template <typename AstNodeT, typename... ArgsT>
AstNodeT &ParserDriver::create(ArgsT &&args) {
  m_node_deleter.emplace_back(
      std::make_unique<AstNode>(std::forward<ArgsT>(args)...));
  AstNodeT *result = dynamic_cast<AstNodeT *>(m_node_deleter.back().get());
  assert(result);
  return *result;
}

} // namespace eta