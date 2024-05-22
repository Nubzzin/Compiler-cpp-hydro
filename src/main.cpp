#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <optional>
#include <ostream>
#include <sstream>
#include <vector>

#include "constructor.hpp"
#include "parser.hpp"
#include "tokenizer.hpp"

int main(int argc, char *argv[]) {
  // TODO remover debugs depois de pronto

  // DEBUG evitando erro no debug
  // if (argc != 2) {
  //   std::cerr << "Uso: hydro [filename.hy]" << std::endl;
  //   return EXIT_FAILURE;
  // }
  // DEBUG

  // Abrindo e verificando se arquivo é valido
  std::fstream file("test.hy" /*argv[1]*/, std::ios::in);
  if (!file.is_open()) {
    if (argv[1] != NULL) {
      std::cerr << "Falha ao encontrar arquivo: " << argv[1] << std::endl;
    } else {
      std::cerr << "Uso: hydro [arquivo.hy]" << std::endl;
    }
    return EXIT_FAILURE;
  }

  // Lendo arquivo
  std::stringstream contents;
  contents << file.rdbuf();
  file.close();

  // Realizando Tokenização
  Tokenizer tokenizer(contents.str());
  std::vector<Token> tokens = tokenizer.tokenize();

  // DEBUG Tokens
  std::cout << "Tokens size: " << tokens.size() << std::endl;

  std::cout << "Tokens: ";
  for (Token c : tokens)
    std::cout << c.value.value() << " ";
  // DEBUG END

  // Parsing tokens
  Parser parser(tokens);
  std::optional<NodeMain> AST = parser.parse();

  // DEBUG NodeExitExpr value
  std::cout << "\nNodeExitExpr value: "
            << std::get<NodeExit>(AST.value().main_value)
                   .exit_value.exit_expr_value->value.value()
            << std::endl;
  // DEBUG END

  // Construindo arquivo
  Constructor construc(AST);
  construc.constructor();

  // Compilando arquivo c
  // Linux
  system("gcc -o aLinux.out a.c");

  return EXIT_SUCCESS;
}
