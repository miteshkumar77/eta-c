#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <iostream>
#include <sstream>

#include <FlexLexer.h>
#include "lexer_test_utils.hpp"

using minic::lexer::ArbitraryLiteralMeta;
using minic::lexer::BoolLiteralMeta;
using minic::lexer::ErrorMeta;
using minic::lexer::IdentifierMeta;
using minic::lexer::StateHandle;
using minic::lexer::token;
using minic::lexer::test::tokenize;

using ::testing::ElementsAre;

TEST(LexerTest, WhiteSpaceTest)
{
  const std::string input(" a ");
  std::istringstream is(input);
  yyFlexLexer fl(&is, &std::cerr);
  int rc{0};
  while ((rc = fl.yylex()) != static_cast<int>(0))
  {
    std::cerr << "lexed rc: " << rc << ", for text: "
              << fl.YYText() << std::endl;
  }
}

TEST(LexerTest, IfKeywordTest)
{
  {
    const std::vector<token> tokens =
        tokenize(" if ");

    ASSERT_THAT(tokens, ElementsAre(token{.tag = MC_IF_KWD,
                                          .meta = {}}));
  }
  {
    const std::vector<token> tokens =
        tokenize(" if\nif if  if\nif\nif ");

    ASSERT_THAT(tokens, ElementsAre(
                            token{
                                .tag = MC_IF_KWD,
                                .meta = {},
                            },
                            token{
                                .tag = MC_IF_KWD,
                                .meta = {},
                            },
                            token{
                                .tag = MC_IF_KWD,
                                .meta = {},
                            },
                            token{
                                .tag = MC_IF_KWD,
                                .meta = {},
                            },
                            token{
                                .tag = MC_IF_KWD,
                                .meta = {},
                            },
                            token{
                                .tag = MC_IF_KWD,
                                .meta = {},
                            }));
  }
}

TEST(LexerTest, Parens)
{
  {
    const std::vector<token> tokens =
        tokenize("( \n \t )");

    ASSERT_THAT(tokens, ElementsAre(
                            token{
                                .tag = MC_LPAR,
                                .meta = {},
                            },
                            token{
                                .tag = MC_RPAR,
                                .meta = {},
                            }));
  }
}

TEST(LexerTest, LineCmt)
{
  {
    const std::vector<token> tokens =
        tokenize("{( //)}}\n)}");
    ASSERT_THAT(tokens, ElementsAre(
                            token{
                                .tag = MC_LBRACE,
                                .meta = {},
                            },
                            token{
                                .tag = MC_LPAR,
                                .meta = {},
                            },
                            token{
                                .tag = MC_RPAR,
                                .meta = {},
                            },
                            token{
                                .tag = MC_RBRACE,
                                .meta = {},
                            }));
  }

  {
    const std::vector<token> tokens =
        tokenize("//)}}\n{( //abcdef123\t\n\t))}");
    ASSERT_THAT(tokens, ElementsAre(
                            token{
                                .tag = MC_LBRACE,
                                .meta = {},
                            },
                            token{
                                .tag = MC_LPAR,
                                .meta = {},
                            },
                            token{
                                .tag = MC_RPAR,
                                .meta = {},
                            },
                            token{
                                .tag = MC_RPAR,
                                .meta = {},
                            },
                            token{
                                .tag = MC_RBRACE,
                                .meta = {},
                            }));
  }
}

TEST(LexerTest, MultiLineCmt)
{

  {
    const std::vector<token> tokens =
        tokenize("{( /*)}}\n)*/)}");
    ASSERT_THAT(tokens, ElementsAre(
                            token{
                                .tag = MC_LBRACE,
                                .meta = {},
                            },
                            token{
                                .tag = MC_LPAR,
                                .meta = {},
                            },
                            token{
                                .tag = MC_RPAR,
                                .meta = {},
                            },
                            token{
                                .tag = MC_RBRACE,
                                .meta = {},
                            }));
  }
  {
    const std::vector<token> tokens =
        tokenize("/\t/*abc\n}}{{}}\n\t\n*//))}");
    ASSERT_THAT(tokens, ElementsAre(
                            token{
                                .tag = MC_DIV_BIN,
                                .meta = {},
                            },
                            token{
                                .tag = MC_DIV_BIN,
                                .meta = {},
                            },
                            token{
                                .tag = MC_RPAR,
                                .meta = {},
                            },
                            token{
                                .tag = MC_RPAR,
                                .meta = {},
                            },
                            token{
                                .tag = MC_RBRACE,
                                .meta = {},
                            }));
  }

  {
    const std::vector<token> tokens =
        tokenize("//*abc\n/*}}{{}}\n\t\n*//))}");
    ASSERT_THAT(tokens, ElementsAre(
                            token{
                                .tag = MC_DIV_BIN,
                                .meta = {},
                            },
                            token{
                                .tag = MC_RPAR,
                                .meta = {},
                            },
                            token{
                                .tag = MC_RPAR,
                                .meta = {},
                            },
                            token{
                                .tag = MC_RBRACE,
                                .meta = {},
                            }));
  }

  {
    const std::vector<token> tokens =
        tokenize("{(/*//\n\n\t+*///)}\n)}");
    ASSERT_THAT(tokens, ElementsAre(
                            token{
                                .tag = MC_LBRACE,
                                .meta = {},
                            },
                            token{
                                .tag = MC_LPAR,
                                .meta = {},
                            },
                            token{
                                .tag = MC_RPAR,
                                .meta = {},
                            },
                            token{
                                .tag = MC_RBRACE,
                                .meta = {},
                            }));
  }
}

struct StateHandleTestBackdoor
{
  template <typename MetaT>
  static StateHandle create(MetaT &&meta)
  {
    return StateHandle::create(std::forward<MetaT>(meta));
  }
};

TEST(LexerTest, IntegerLiteral)
{
  {
    const std::vector<token> tokens =
        tokenize("( 123 1 0 1\n1)223");

    ASSERT_THAT(tokens, ElementsAre(
                            token{
                                .tag = MC_LPAR,
                                .meta = {},
                            },
                            token{
                                .tag = MC_INTEGER_LITERAL,
                                .meta = {StateHandleTestBackdoor::create(
                                    ArbitraryLiteralMeta{.content = "123"})},
                            },
                            token{
                                .tag = MC_INTEGER_LITERAL,
                                .meta = {StateHandleTestBackdoor::create(
                                    ArbitraryLiteralMeta{.content = "1"})},
                            },
                            token{
                                .tag = MC_INTEGER_LITERAL,
                                .meta = {StateHandleTestBackdoor::create(
                                    ArbitraryLiteralMeta{.content = "0"})},
                            },
                            token{
                                .tag = MC_INTEGER_LITERAL,
                                .meta = {StateHandleTestBackdoor::create(
                                    ArbitraryLiteralMeta{.content = "1"})},
                            },
                            token{
                                .tag = MC_INTEGER_LITERAL,
                                .meta = {StateHandleTestBackdoor::create(
                                    ArbitraryLiteralMeta{.content = "1"})},
                            },
                            token{
                                .tag = MC_RPAR,
                                .meta = {},
                            },
                            token{
                                .tag = MC_INTEGER_LITERAL,
                                .meta = {StateHandleTestBackdoor::create(
                                    ArbitraryLiteralMeta{.content = "223"})},
                            }));
  }
}

TEST(LexerTest, FloatLiteral)
{
  {
    const std::vector<token> tokens =
        tokenize("( 123.1234 1.0 0.1 1.\n1.13)223.90");

    ASSERT_THAT(tokens, ElementsAre(
                            token{
                                .tag = MC_LPAR,
                                .meta = {},
                            },
                            token{
                                .tag = MC_FLOAT_LITERAL,
                                .meta = {StateHandleTestBackdoor::create(
                                    ArbitraryLiteralMeta{.content = "123.1234"})},
                            },
                            token{
                                .tag = MC_FLOAT_LITERAL,
                                .meta = {StateHandleTestBackdoor::create(
                                    ArbitraryLiteralMeta{.content = "1.0"})},
                            },
                            token{
                                .tag = MC_FLOAT_LITERAL,
                                .meta = {StateHandleTestBackdoor::create(
                                    ArbitraryLiteralMeta{.content = "0.1"})},
                            },
                            token{
                                .tag = MC_FLOAT_LITERAL,
                                .meta = {StateHandleTestBackdoor::create(
                                    ArbitraryLiteralMeta{.content = "1."})},
                            },
                            token{
                                .tag = MC_FLOAT_LITERAL,
                                .meta = {StateHandleTestBackdoor::create(
                                    ArbitraryLiteralMeta{.content = "1.13"})},
                            },
                            token{
                                .tag = MC_RPAR,
                                .meta = {},
                            },
                            token{
                                .tag = MC_FLOAT_LITERAL,
                                .meta = {StateHandleTestBackdoor::create(
                                    ArbitraryLiteralMeta{.content = "223.90"})},
                            }));
  }
}

TEST(LexerTest, StringLiteral)
{
  {
    const std::vector<token> tokens =
        tokenize("/+-/%\"ab//a/*b*/1\"//\"aba//b1\"\n}");
    ASSERT_THAT(tokens, ElementsAre(
                            token{
                                .tag = MC_DIV_BIN,
                                .meta = {},
                            },
                            token{
                                .tag = MC_PLUS_BIN,
                                .meta = {},
                            },
                            token{
                                .tag = MC_MINUS_BIN,
                                .meta = {},
                            },
                            token{
                                .tag = MC_DIV_BIN,
                                .meta = {},
                            },
                            token{
                                .tag = MC_MOD_BIN,
                                .meta = {},
                            },
                            token{
                                .tag = MC_STRING_LITERAL,
                                .meta = StateHandleTestBackdoor::create(ArbitraryLiteralMeta{
                                    .content = std::string("ab//a/*b*/1")}),
                            },
                            token{
                                .tag = MC_RBRACE,
                                .meta = {},
                            }));
  }
  {
    const std::vector<token> tokens =
        tokenize("/+-/%\"ab//a/*b*/1\\f\\t\\n\\bq\\\"\"//\"aba//b1\"\n}");
    ASSERT_THAT(tokens, ElementsAre(
                            token{
                                .tag = MC_DIV_BIN,
                                .meta = {},
                            },
                            token{
                                .tag = MC_PLUS_BIN,
                                .meta = {},
                            },
                            token{
                                .tag = MC_MINUS_BIN,
                                .meta = {},
                            },
                            token{
                                .tag = MC_DIV_BIN,
                                .meta = {},
                            },
                            token{
                                .tag = MC_MOD_BIN,
                                .meta = {},
                            },
                            token{
                                .tag = MC_STRING_LITERAL,
                                .meta = StateHandleTestBackdoor::create(ArbitraryLiteralMeta{
                                    .content = std::string("ab//a/*b*/1\f\t\n\bq\"")}),
                            },
                            token{
                                .tag = MC_RBRACE,
                                .meta = {},
                            }));
  }
}

TEST(LexerTest, BoolLiteral)
{
  {
    const std::vector<token> tokens =
        tokenize("{true false}");
    ASSERT_THAT(tokens, ElementsAre(
                            token{
                                .tag = MC_LBRACE,
                                .meta = {}},
                            token{
                                .tag = MC_BOOL_LITERAL,
                                .meta = StateHandleTestBackdoor::create(BoolLiteralMeta{.value = true}),
                            },
                            token{
                                .tag = MC_BOOL_LITERAL,
                                .meta = StateHandleTestBackdoor::create(BoolLiteralMeta{.value = false}),
                            },
                            token{
                                .tag = MC_RBRACE,
                                .meta = {}}));
  }
}

TEST(LexerTest, IdentifierTest)
{
  {
    const std::vector<token> tokens =
        tokenize("{truefalse; true; false; ifelse if() else()}");
    ASSERT_THAT(tokens, ElementsAre(
                            token{
                                .tag = MC_LBRACE,
                                .meta = {},
                            },
                            token{
                                .tag = MC_IDENTIFIER,
                                .meta = StateHandleTestBackdoor::create(IdentifierMeta{.name = std::string("truefalse")}),
                            },
                            token{
                                .tag = MC_SEMI,
                                .meta = {},
                            },
                            token{
                                .tag = MC_BOOL_LITERAL,
                                .meta = {StateHandleTestBackdoor::create(BoolLiteralMeta{.value = true})},
                            },
                            token{
                                .tag = MC_SEMI,
                                .meta = {},
                            },
                            token{
                                .tag = MC_BOOL_LITERAL,
                                .meta = {StateHandleTestBackdoor::create(BoolLiteralMeta{.value = false})},
                            },
                            token{
                                .tag = MC_SEMI,
                                .meta = {},
                            },
                            token{
                                .tag = MC_IDENTIFIER,
                                .meta = StateHandleTestBackdoor::create(IdentifierMeta{.name = std::string("ifelse")}),
                            },
                            token{
                                .tag = MC_IF_KWD,
                                .meta = {},
                            },
                            token{
                                .tag = MC_LPAR,
                                .meta = {},
                            },
                            token{
                                .tag = MC_RPAR,
                                .meta = {},
                            },
                            token{
                                .tag = MC_ELSE_KWD,
                                .meta = {},
                            },
                            token{
                                .tag = MC_LPAR,
                                .meta = {},
                            },
                            token{
                                .tag = MC_RPAR,
                                .meta = {},
                            },
                            token{
                                .tag = MC_RBRACE,
                                .meta = {},
                            }));
  }
}