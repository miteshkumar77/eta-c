%{
  #include "parser/lib_eta_parser.h"
  #include "base/defs.hpp"

  void yy::parser::error(const std::string&)
  {}

  static bool needs_reset_in_next_yy_action { true };
  void reset_symbol_to_parser(char const*const yytext);

  #define YY_USER_ACTION \
    reset_symbol_to_parser(yytext);

  #define MAX_STR_CONST 1025
  #define YY_NO_UNPUT

  #define ETATOK(name) yy::parser::token_type::TOK_##name

  struct temp_nullterm
  {
    temp_nullterm(char& c);
    ~temp_nullterm();

    char prev_;
    char& c_;
  };

  void lex_special_char(char const*const yytext);

  static int64_t s_eta_line_number{0}; // static: only written in lib_eta_lexer.cc
  static int64_t s_eta_column_number{0}; // static: only written in lib_eta_lexer.cc

%}

WHITESPACE (" "|\f|\r|\t|\v|\n)
LINE_CMT "//"
IDENTIFIER [a-zA-Z][a-zA-Z0-9_']*
DIGIT [0-9]
INTEGER_LITERAL {DIGIT}+
UNICODE_CHAR \\x\{[0-9a-fA-F]+\}

%x LINE_CMT_SCOPE
%x STR_LITERAL_SCOPE

%%

{WHITESPACE} {/*nothing*/}

{LINE_CMT} {
  BEGIN(LINE_CMT_SCOPE);
}
<LINE_CMT_SCOPE>\n {
  BEGIN(INITIAL);
}
<LINE_CMT_SCOPE>[^\n] {
  /* nothing */
}
"\"" {
  BEGIN(STR_LITERAL_SCOPE);
}
<STR_LITERAL_SCOPE>\n { // don't allow escaped multi-line either, so put this first
  BEGIN(INITIAL);
  eta::symbol_to_parser.error = "multi-line string literal not allowed";
  return ETATOK(ERROR);
}
<STR_LITERAL_SCOPE>\\[^x\n] {
  lex_special_char(yytext);
}
<STR_LITERAL_SCOPE>"\"" {
  BEGIN(INITIAL);
  return ETATOK(STR_LITERAL);
}
<STR_LITERAL_SCOPE>{UNICODE_CHAR} {
  needs_reset_in_next_yy_action = false;
  std::optional<eta::ustr_util::err> ec = eta::ustr_util::append_unicode_char(eta::symbol_to_parser.uc_content, yytext);
}
<STR_LITERAL_SCOPE>. {
  needs_reset_in_next_yy_action = false;
  eta::ustr_util::append_ascii_chars(eta::symbol_to_parser.uc_content, yytext);
}

if { return ETATOK(IF_KWD); }
else { return ETATOK(ELSE_KWD); }
while { return ETATOK(WHILE_KWD); }
use { return ETATOK(USE_KWD); }
return { return ETATOK(RETURN_KWD); }
length { return ETATOK(LENGTH_KWD); }
"(" { return ETATOK(LPAREN); }
")" { return ETATOK(RPAREN); }
"[" { return ETATOK(LBRACKET); }
"]" { return ETATOK(RBRACKET); }
"{" { return ETATOK(LBRACE); }
"}" { return ETATOK(RBRACE); }

":" { return ETATOK(COLON); }
";" { return ETATOK(SEMICOLON); }
"," { return ETATOK(COMMA); }

int { return ETATOK(INT_TYPE); }
bool { return ETATOK(BOOL_TYPE); }
true { return ETATOK(TRUE_LITERAL); }
false { return ETATOK(FALSE_LITERAL); }

{IDENTIFIER} { 
  eta::symbol_to_parser.identifier_name = yytext;
  return ETATOK(IDENTIFIER);
}
'\\.' {
  [[maybe_unused]] temp_nullterm _(yytext[yyleng-1]);
  lex_special_char(yytext+1);
  return ETATOK(CHAR_LITERAL);
}
'{UNICODE_CHAR}' {
  [[maybe_unused]] temp_nullterm _(yytext[yyleng-1]);
  std::optional<eta::ustr_util::err> ec = eta::ustr_util::append_unicode_char(eta::symbol_to_parser.uc_content, yytext+1);
  return ETATOK(CHAR_LITERAL);
}
'' {
  eta::symbol_to_parser.error = "Invalid character constant";
  return ETATOK(ERROR);
}

"="   { return ETATOK(OP_ASSIGN); }
"-"   { return ETATOK(OP_MINUS); }
"!"   { return ETATOK(OP_LNOT); }
"*>>" { return ETATOK(OP_HIGHMUL); }
"*"   { return ETATOK(OP_MUL); }
"/"   { return ETATOK(OP_DIV); }
"%"   { return ETATOK(OP_MOD); }
"+"   { return ETATOK(OP_ADD); }
"<="  { return ETATOK(OP_LEQ_THAN); }
"<"   { return ETATOK(OP_L_THAN); }
">="  { return ETATOK(OP_GEQ_THAN); }
">"   { return ETATOK(OP_G_THAN); }
"=="  { return ETATOK(OP_EQ); }
"!="  { return ETATOK(OP_NEQ); }
"&"   { return ETATOK(OP_LAND); }
"|"   { return ETATOK(OP_LOR); }

{INTEGER_LITERAL} {
  const std::variant<int64_t, eta::int_util::err> decoded = eta::int_util::from_str(yytext);
  if (std::holds_alternative<eta::int_util::err>(decoded)) {
    eta::symbol_to_parser.error = "invalid int literal";
    return ETATOK(ERROR);
  }

  eta::symbol_to_parser.int_content = std::get<int64_t>(decoded);

  return ETATOK(INT_LITERAL);
}

<<EOF>> { return 0; }

. {
  eta::symbol_to_parser.error = "unknown character";
  return ETATOK(ERROR);
}

%%

#undef ETATOK

void calc_positions(char const*const yytext)
{
  for (char c : std::string_view{yytext}) { // goes till nullptr
    if (c == '\n') {
      ++s_eta_line_number;
      s_eta_column_number = 1;
    } else {
      ++s_eta_column_number;
    }
  }
}

void reset_symbol_to_parser(char const*const yytext)
{
  if (eta::symbol_to_parser.is_for_first_token()) {
    s_eta_column_number = 1;
    s_eta_line_number = 1;
  }
  if (needs_reset_in_next_yy_action) {
    eta::symbol_to_parser.prime_for_next_token(s_eta_line_number, s_eta_column_number);
  }
  calc_positions(yytext);
  needs_reset_in_next_yy_action = true;
}

void lex_special_char(char const*const yytext)
{
  needs_reset_in_next_yy_action = false;
  const char special_char = yytext[1];
  static constexpr char newline[] = "\n";
  static constexpr char tab[] = "\t";
  static constexpr char bslash[] = "\\";
  static constexpr char charquote[] = "'";
  switch (special_char)
  {
    case 'n':
      eta::ustr_util::append_ascii_chars(eta::symbol_to_parser.uc_content, newline);
      break;
    case 't':
      eta::ustr_util::append_ascii_chars(eta::symbol_to_parser.uc_content, tab);
      break;
    case '\\':
      eta::ustr_util::append_ascii_chars(eta::symbol_to_parser.uc_content, bslash);
      break;
    case '\'':
      eta::ustr_util::append_ascii_chars(eta::symbol_to_parser.uc_content, charquote);
      break;
    default:
      eta::ustr_util::append_ascii_chars(eta::symbol_to_parser.uc_content, (yytext + 1));
  }
}

temp_nullterm::temp_nullterm(char& c) : prev_{c}, c_{c}
{
  c_ = 0;
}

temp_nullterm::~temp_nullterm()
{
  c_ = prev_;
}

int64_t eta_line_number()
{
  return s_eta_line_number;
}

int64_t eta_column_number()
{
  return s_eta_column_number;
}