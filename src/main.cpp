#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <optional>
#include <ostream>
#include <sstream>
#include <vector>

#include "parser.h"
#include "tokenizer.h"

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Muitos argumentos!" << std::endl;
  }

  // Test time
  std::fstream file(/*argv[1]*/ "test.hy", std::ios::in);
  std::stringstream contents;
  if (!file.is_open()) {
    std::cerr << "Falha ao encontrar arquivo: " << argv[1] << std::endl;
    return EXIT_FAILURE;
  }
  contents << file.rdbuf();
  file.close();

  Tokenizer tokenizer(contents.str());
  std::vector<Token> tokens = tokenizer.tokenize();

  std::cout << "Tokens size: " << tokens.size() << std::endl;

  for (Token c : tokens)
    std::cout << c.value.value() << " ";

  Parser parser(tokens);
  std::optional<NodeBody> ne = parser.parse();

  std::cout << "\n" << ne.value().values.size() << std::endl;

  return 0;
}
