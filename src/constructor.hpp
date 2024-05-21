#ifndef _CONSTRUCTOR_H_
#define _CONSTRUCTOR_H_

#include <fstream>
#include <optional>

#include "parser.hpp"
#include "tokenizer.hpp"

class Constructor {
private:
  const std::optional<NodeMain> AST;
  size_t index{};

public:
  Constructor(std::optional<NodeMain> &AST) : AST{std::move(AST.value())} {}
  ~Constructor() = default;

  void constructor() {
    std::fstream file("a.c", std::ios::out);
    file << "#include <stdlib.h>\n";
    if (AST.has_value()) {
      file << "int main() {";
      if (std::get<NodeExit>(AST->main_value)
              .exit_value.exit_expr_value.has_value()) {
        file << "exit(";
        file << std::get<NodeExit>(AST.value().main_value)
                    .exit_value.exit_expr_value->value.value();
      }
      file << ");";
    }
    file << "}";
  }
}

;

#endif
