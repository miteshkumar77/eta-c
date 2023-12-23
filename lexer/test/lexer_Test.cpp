#include <gtest/gtest.h>

#include <iostream>
#include <sstream>

#include <FlexLexer.h>

TEST(LexerTest, IdentifierRegexTest)
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