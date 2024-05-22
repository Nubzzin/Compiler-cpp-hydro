#ifndef _CONSTRUCTOR_H_
#define _CONSTRUCTOR_H_

#include <fstream>
#include <optional>

#include "parser.hpp"
#include "tokenizer.hpp"

class Constructor {
private:
  const NodeMain AST;
  size_t index{};

  // Lendo item no index atual
  std::optional<NodeMainExpr> peek(size_t offset = 0) {
    if (AST.main_values.size() > (index + offset)) {
      return AST.main_values[index + offset];
    } else {
      return {};
    }
  }

  // Lendo item no index atual e em seguido passando para o proximo
  std::optional<NodeMainExpr> consume() {
    if (AST.main_values.size() > index) {
      return AST.main_values[index++];
    } else {
      return {};
    }
  }

public:
  Constructor(std::optional<NodeMain> &AST) : AST{std::move(AST.value())} {}
  ~Constructor() = default;

  void constructor() {
    std::fstream file("a.c", std::ios::out);
    file << "#include <stdlib.h>\n";
    file << "#include <stdio.h>\n";
    if (AST.main_values.size() > 0) {
      file << "int main() {";

      while (peek().has_value()) {
        if (std::get<NodeExit>(peek().value().main_expr_value)
                .exit_value.exit_expr_value.has_value()) {
          file << "exit("
               << std::get<NodeExit>(peek().value().main_expr_value)
                      .exit_value.exit_expr_value->value.value()
               << ");";
        }
        consume();
      }
    }
    file << "}";
  }
};

#endif
