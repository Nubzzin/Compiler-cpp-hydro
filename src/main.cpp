#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "parser.h"
#include "tokenizer.h"

int main(int argc, char *argv[]) {
  std::fstream file("test.hy");
  std::stringstream buf;
  {
    if (!file.is_open()) {
      std::cerr << "Falha ao encontrar arquivo!" << std::endl;
      return EXIT_FAILURE;
    }

    std::string str_file;
    std::getline(file, str_file);
    buf << str_file;
  }

  Tokenizer tokenizer(buf.str());
  std::vector<Token> tokens = tokenizer.tokenize();

  std::cout << tokens.size() << std::endl;
  return 0;
}
