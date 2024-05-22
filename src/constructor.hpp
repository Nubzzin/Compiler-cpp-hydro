#ifndef _CONSTRUCTOR_H_
#define _CONSTRUCTOR_H_

#include <fstream>
#include <optional>
#include <variant>

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
    file << "int main() {";
    while (peek().has_value()) {
      std::cout << "Index: " << peek().value().expr_value.index() << std::endl;
      if (peek()->expr_value.index() == 0) {
        file << "exit(";
        file << std::get<NodeExit>(peek()->expr_value)
                    .exit_value.expr_value->value.value();
        file << ");";

        std::cout << "\nEXIT ENTRY" << std::endl;
      }
      if (peek()->expr_value.index() == 1) {
        if (std::get<NodePrint>(peek()->expr_value)
                .print_value.expr_value->type == TokenType::Str) {
          file << "printf(\"%s\",\"";
        } else if (std::get<NodePrint>(peek()->expr_value)
                       .print_value.expr_value->type == TokenType::IntLit) {
          file << "printf(\"%d\",";
        }

        file << std::get<NodePrint>(peek()->expr_value)
                    .print_value.expr_value->value.value();

        if (std::get<NodePrint>(peek()->expr_value)
                .print_value.expr_value->type == TokenType::Str) {
          file << "\");";
        }
        if (std::get<NodePrint>(peek()->expr_value)
                .print_value.expr_value->type == TokenType::IntLit) {
          file << ");";
        }

        std::cout << "\nPRINT ENTRY" << std::endl;
      }
      consume();
    }
    file << "}";
  }
};

#endif
