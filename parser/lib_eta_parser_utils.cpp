#include "lib_eta_parser_utils.hpp"
namespace eta {

char const *token_to_string(yy::parser::token::yytokentype t) {
  switch (t) {
#define CASE(name)                                                             \
  case yy::parser::token::yytokentype::TOK_##name:                             \
    return "TOK_" #name
    CASE(IDENTIFIER);
    CASE(USE_KWD);
    CASE(WHILE_KWD);
    CASE(IF_KWD);
    CASE(ELSE_KWD);
    CASE(RETURN_KWD);
    CASE(LENGTH_KWD);
    CASE(LPAREN);
    CASE(RPAREN);
    CASE(LBRACKET);
    CASE(RBRACKET);
    CASE(LBRACE);
    CASE(RBRACE);
    CASE(COLON);
    CASE(SEMICOLON);
    CASE(COMMA);
    CASE(INT_TYPE);
    CASE(BOOL_TYPE);
    CASE(TRUE_LITERAL);
    CASE(FALSE_LITERAL);
    CASE(STR_LITERAL);
    CASE(CHAR_LITERAL);
    CASE(INT_LITERAL);
    CASE(OP_ASSIGN);
    CASE(OP_MINUS);
    CASE(OP_LNOT);
    CASE(OP_MUL);
    CASE(OP_HIGHMUL);
    CASE(OP_DIV);
    CASE(OP_MOD);
    CASE(OP_ADD);
    CASE(OP_L_THAN);
    CASE(OP_LEQ_THAN);
    CASE(OP_GEQ_THAN);
    CASE(OP_G_THAN);
    CASE(OP_EQ);
    CASE(OP_NEQ);
    CASE(OP_LAND);
    CASE(OP_LOR);
    CASE(ERROR);
#undef CASE
  }
  return "<unknown>";
}
} // namespace eta
