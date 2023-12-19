#pragma once

#include "parsing_utils/ctpg.hpp"


namespace lexer {

namespace tokens {

constexpr char identifier_pattern[] = R"_("[a-zA-Z_][a-zA-Z0-9_]*")_";
ctpg::regex_term<identifier_pattern> identifier("identifier");

/// delimiters
ctpg::char_term lbra('{');
ctpg::char_term rbra('}');
ctpg::char_term lpar('(');
ctpg::char_term rpar(')');
ctpg::char_term lsbr('[');
ctpg::char_term rsbr(']');
ctpg::char_term sc(';');
ctpg::char_term comma(',');

/// keywords
// types
ctpg::string_term int_t("int");
ctpg::string_term void_t("void");
ctpg::string_term char_t("char");

// keywords
ctpg::string_term if_k("if");
ctpg::string_term else_k("else");
ctpg::string_term while_k("while");
ctpg::string_term return_k("return");
ctpg::string_term struct_k("struct");
ctpg::string_term sizeof_k("sizeof");
ctpg::string_term include_k("#include");

constexpr char string_literal_pattern[] = R"_("[\"]([^\"]|\\\\.|\\\\n)*[\"]")_";
ctpg::regex_term<string_literal_pattern> string_literal("string_literal");
constexpr char int_literal_pattern[] = R"_("[0-9]+")_";
ctpg::regex_term<int_literal_pattern> int_literal("int_literal");
constexpr char char_literal_pattern[] = R"_("'([a-z]|[A-Z]|\\\\t|\\\\b|\\\\n|\\\\r|\\\\f|\\\\'|\\\\"|\\\\\\|\\\\0|.|,|_)'")_";
ctpg::regex_term<char_literal_pattern> char_literal("char_literal");

}
}