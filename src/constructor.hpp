#ifndef _CONSTRUCTOR_H_
#define _CONSTRUCTOR_H_

#include <fstream>
#include <optional>

#include "parser.hpp"
#include "tokenizer.hpp"

class Constructor {
private:
  const std::optional<NodeExit> AST;
  size_t index{};

public:
  Constructor(std::optional<NodeExit> &AST) : AST{std::move(AST.value())} {}
  ~Constructor() = default;

  void constructor() {
    std::fstream file("a.c", std::ios::out);
    file << "#include <stdlib.h>\n";
    file << "int main() {";
    if (AST.has_value()) {
      file << "exit(";
      if (std::get<NodeExitExpr>(AST.value().exitvalue)
              .exitExprvalue.has_value()) {
        file << std::get<NodeExitExpr>(AST.value().exitvalue)
                    .exitExprvalue->value.value();
      }
      file << ");";
    }
    file << "}";
  }
};

#endif
