#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <iostream>
#include <sstream>

#include <FlexLexer.h>
#include "lexer_test_utils.hpp"

using ::testing::ElementsAre;
using minic::lexer::test::tokenize;
using minic::lexer::token;

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
                                    .tag= MC_IF_KWD,
                                    .meta = {},
                                },
                                token{
                                    .tag= MC_IF_KWD,
                                    .meta = {},
                                },
                                token{
                                    .tag= MC_IF_KWD,
                                    .meta = {},
                                },
                                token{
                                    .tag= MC_IF_KWD,
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
        }
    ));
    }
}