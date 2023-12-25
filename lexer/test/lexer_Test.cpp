#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <iostream>
#include <sstream>

#include <FlexLexer.h>
#include "lexer_test_utils.hpp"

using minic::lexer::token;
using minic::lexer::test::tokenize;
using minic::lexer::StateHandle;
using minic::lexer::StringLiteralMeta;
using minic::lexer::ErrorMeta;

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
            tokenize(" ifif if  ififif ");

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
                                    .meta = {},
                                },
                                token{
                                    .tag = MC_INTEGER_LITERAL,
                                    .meta = {},
                                },
                                token{
                                    .tag = MC_INTEGER_LITERAL,
                                    .meta = {},
                                },
                                token{
                                    .tag = MC_INTEGER_LITERAL,
                                    .meta = {},
                                },
                                token{
                                    .tag = MC_INTEGER_LITERAL,
                                    .meta = {},
                                },
                                token{
                                    .tag = MC_RPAR,
                                    .meta = {},
                                },
                                token{
                                    .tag = MC_INTEGER_LITERAL,
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
            }
        ));
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
            }
        ));
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
            }
        ));
    }
}

struct StateHandleTestBackdoor
{
    template <typename MetaT>
    static StateHandle create(MetaT&& meta)
    {
        return StateHandle::create(std::forward<MetaT>(meta));
    }
};

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
                .meta = StateHandleTestBackdoor::create(StringLiteralMeta{
                    .content=std::string("ab//a/*b*/1")
                }),
            },
            token{
                .tag = MC_RBRACE,
                .meta = {},
            }
        ));
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
                .meta = StateHandleTestBackdoor::create(StringLiteralMeta{
                    .content=std::string("ab//a/*b*/1\f\t\n\bq\"")
                }),
            },
            token{
                .tag = MC_RBRACE,
                .meta = {},
            }
        ));
    }
}