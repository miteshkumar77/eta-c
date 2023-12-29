#include "parser/minic_parser.h"

int main(int argc, char** argv)
{
  if (argc != 2) {
    std::cerr << "ERROR: usage: ./main <path_to_lexer_input>" << std::endl;
    return 0;
  }
  std::ifstream fs;
  fs.open(argv[1], std::ifstream::in);
  // todo
}