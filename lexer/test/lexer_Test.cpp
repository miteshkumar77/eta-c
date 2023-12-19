#include <gtest/gtest.h>

#include <compiler/lexer.hpp>
#include <compiler/tokens.hpp>

#include <iostream>

TEST(LexerTest, TokenizerTest) {
    constexpr auto pattern = lexer::tokens::identifier.get_data();
    constexpr ctpg::regex::expr<pattern.pattern> r;
    ASSERT_TRUE(r.match(ctpg::match_options{}.set_verbose(), ctpg::buffers::string_buffer("abc123"), std::cout));
}