#pragma once

#include "parser/lib_eta_parser.h"

namespace eta {
char const *token_to_string(yy::parser::token::yytokentype token);
}