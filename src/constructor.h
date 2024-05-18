#ifndef _CONSTRUCTOR_H_
#define _CONSTRUCTOR_H_

#include <fstream>
#include <string>
#include <vector>

#include "parser.h"

class Constructor {
private:
  const NodeExit AST;
  size_t index{};

public:
  Constructor(std::optional<NodeExit> &AST) : AST{std::move(AST.value())} {}
  ~Constructor() = default;

  void constructor() {
    std::fstream file("a.cpp", std::ios::out);
    file << "#include <cstdlib>\n";
    file << "int main() {";
    if (!AST.value.valueless_by_exception()) {
      file << "exit(";
      if (std::get<NodeExitExpr>(AST.value).value.has_value()) {
        file << std::get<NodeExitExpr>(AST.value).value->value.value();
      }
      file << ");";
    }
    file << "}";
  }
};

#endif
