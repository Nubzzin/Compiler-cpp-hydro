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
  // DEBUG
  // if (argc != 2) {
  //   std::cerr << "Muitos argumentos!" << std::endl;
  //   return EXIT_FAILURE;
  // }

  // Test time
  std::fstream file(argv[1], std::ios::in);
  if (!file.is_open()) {
    if (argv[1] != NULL) {
      std::cerr << "Falha ao encontrar arquivo: " << argv[1] << std::endl;
    } else {
      std::cerr << "Uso: hydro [arquivo.hy]" << std::endl;
    }
    return EXIT_FAILURE;
  }

  std::stringstream contents;
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
