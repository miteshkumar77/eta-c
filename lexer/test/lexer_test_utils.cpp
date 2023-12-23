#include "lexer_test_utils.hpp"

#include <boost/iostreams/stream.hpp>
#include <FlexLexer.h>
#include <iostream>

std::vector<token> tokenize(const std::string_view input)
{
    std::vector<token> rval;
    boost::iostreams::stream<boost::iostreams::array_source> is(input.data(), input.size());
    yyFlexLexer fl(&is, &std::cerr);
    int rc { 0 };
    while ((rc = fl.yylex()) != static_cast<int>(0))
    {
        rval.emplace_back(token{
            .token_type = static_cast<minic_tokentype>(rc),
            .token_meta=minic_state_handle,
        });
    }
    return rval;
}