
#include "tools/cpp/runfiles/runfiles.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <iostream>
#include <sstream>

#include "lexer/test/test_lexer.hpp"

using bazel::tools::cpp::runfiles::Runfiles;
using namespace eta;
using namespace eta::test;

using testing::ElementsAre;

class LexerTest : public testing::Test {
protected:
  void SetUp() override {
    std::string error;
    runfiles = std::unique_ptr<Runfiles>(Runfiles::CreateForTest(&error));
    ASSERT_TRUE(runfiles) << "unable to open runfiles, error: " << error;
  }
  std::vector<LexerRval> tokenize_filename(const std::string &name) {
    static const std::string prefix = "etac/lexer/test/data/";
    const std::string fullFilePath = runfiles->Rlocation(prefix + name);
    return tokenize_file(fullFilePath);
  }
  std::unique_ptr<Runfiles> runfiles{nullptr};
};

#define BT(name)                                                               \
  LexerRval {                                                                  \
    .type = yy::parser::token::yytokentype::TOK_##name, .meta = TokenMeta {}   \
  }
#define ID(str)                                                                \
  LexerRval {                                                                  \
    .type = yy::parser::token::yytokentype::TOK_IDENTIFIER,                    \
    .meta = TokenMeta {                                                        \
      .identifier_name = str                                                   \
    }                                                                          \
  }
#define IL(str)                                                                \
  LexerRval {                                                                  \
    .type = yy::parser::token::yytokentype::TOK_INT_LITERAL,                   \
    .meta = TokenMeta {                                                        \
      .int_content = str                                                       \
    }                                                                          \
  }

#define SL(str)                                                                \
  LexerRval {                                                                  \
    .type = yy::parser::token::yytokentype::TOK_STR_LITERAL,                   \
    .meta = TokenMeta {                                                        \
      .uc_content = eta::util::to_wstring(str)                                 \
    }                                                                          \
  }

#define CL(str)                                                                \
  LexerRval {                                                                  \
    .type = yy::parser::token::yytokentype::TOK_CHAR_LITERAL,                  \
    .meta = TokenMeta {                                                        \
      .uc_content = eta::util::to_wstring(str)                                 \
    }                                                                          \
  }

TEST_F(LexerTest, BasicTest) {

  EXPECT_THAT(tokenize_filename("all_basic_tokens.eta"),
              ElementsAre(BT(USE_KWD), BT(COLON), BT(INT_TYPE), BT(OP_ASSIGN),
                          BT(SEMICOLON), BT(BOOL_TYPE), BT(LPAREN), BT(RPAREN),
                          BT(LBRACKET), BT(WHILE_KWD), BT(COMMA), BT(IF_KWD),
                          BT(ELSE_KWD), BT(RETURN_KWD), BT(RBRACKET),
                          BT(OP_MINUS), BT(OP_NEQ), BT(OP_LNOT), BT(OP_MUL),
                          BT(OP_HIGHMUL), BT(OP_MUL), BT(OP_G_THAN), BT(OP_DIV),
                          BT(OP_MOD), BT(OP_ADD), BT(OP_MINUS), BT(OP_L_THAN),
                          BT(OP_LEQ_THAN), BT(OP_GEQ_THAN), BT(OP_G_THAN),
                          BT(OP_EQ), BT(OP_LAND), BT(OP_LOR), BT(LENGTH_KWD),
                          BT(TRUE_LITERAL), BT(FALSE_LITERAL)));
}

TEST_F(LexerTest, IdentifierTest) {
  EXPECT_THAT(tokenize_filename("identifiers.eta"),
              ElementsAre(BT(IF_KWD), BT(ELSE_KWD), BT(WHILE_KWD), BT(USE_KWD),
                          BT(RETURN_KWD), BT(INT_TYPE), BT(BOOL_TYPE),
                          ID("intbool"), ID("int'"), ID("int_"), ID("Int_"),
                          ID("IntABC1'2_3"), BT(OP_LNOT), ID("b")));
}

TEST_F(LexerTest, IntLiteralTest) {
  EXPECT_THAT(tokenize_filename("integer_literals.eta"),
              ElementsAre(BT(LBRACE), IL(1), BT(COMMA), IL(2), BT(COMMA),
                          ID("a12"), BT(COMMA), ID("A12"), BT(COMMA),
                          ID("b'12"), BT(COMMA), IL(4), BT(COMMA), IL(100),
                          BT(OP_MINUS), IL(2), BT(COMMA), BT(OP_MINUS), IL(20),
                          BT(RBRACE), BT(SEMICOLON), IL(10), BT(OP_MINUS),
                          IL(2), BT(SEMICOLON)));
}

TEST_F(LexerTest, StrLiteralTest) {
  EXPECT_THAT(
      tokenize_filename("string_literals.eta"),
      ElementsAre(BT(INT_TYPE), BT(LBRACKET), BT(RBRACKET), BT(OP_ASSIGN),
                  SL("ascii string literal"), BT(SEMICOLON), BT(INT_TYPE),
                  BT(LBRACKET), BT(RBRACKET), BT(OP_ASSIGN),
                  SL("string with newline \n newline \t \\ \" '' "),
                  BT(SEMICOLON), BT(INT_TYPE), BT(LBRACKET), BT(RBRACKET),
                  BT(OP_ASSIGN), SL("string with a unicode char \\\n"),
                  BT(SEMICOLON), BT(INT_TYPE), BT(LBRACKET), BT(RBRACKET),
                  BT(OP_ASSIGN), BT(LBRACE), CL("\n"), BT(COMMA), ID("waa"),
                  BT(COMMA), CL("\'"), BT(COMMA), CL("\n"), BT(RBRACE),
                  BT(SEMICOLON)));
}