#include "lexer/test/test_lexer.hpp"

#include "tools/cpp/runfiles/runfiles.h"

#include <iostream>
#include <memory>

using bazel::tools::cpp::runfiles::Runfiles;
using namespace eta;
using namespace eta::test;

std::vector<LexerRval> tokenize_filename(const std::string &name,
                                         const Runfiles &rf);

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cerr << "USAGE: ./main_exe <name_in_data_dir>+" << std::endl;
    return EXIT_FAILURE;
  }
  std::string rf_error;
  std::unique_ptr<Runfiles> rfs{Runfiles::Create(argv[0], rf_error)};
  if (!rfs.get()) {
    std::cerr << "ERROR: bazel::tools::cpp::runfiles::Runfiles::Create failed "
                 "with error: "
              << rf_error << std::endl;
    return EXIT_FAILURE;
  }
  for (int i = 1; i < argc; ++i) {
    char const *const lex_input_name = argv[i];
    std::wcout << "~~~~~~~~~~ begin lexer output for " << lex_input_name
               << " ~~~~~~~~~~" << std::endl;
    for (const auto &v : tokenize_filename(lex_input_name, *rfs)) {
      std::wcout << v << std::endl;
    }
    std::wcout << "~~~~~~~~~~ end lexer output for " << lex_input_name
               << "  ~~~~~~~~~~" << std::endl;
  }
  return EXIT_SUCCESS;
}

std::vector<LexerRval> tokenize_filename(const std::string &name,
                                         const Runfiles &rfs) {
  static const std::string prefix = "etac/lexer/test/data/";
  const std::string fullFilePath = rfs.Rlocation(prefix + name);
  return tokenize_file(fullFilePath);
}