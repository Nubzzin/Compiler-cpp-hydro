#ifndef _CONSTRUCTOR_H_
#define _CONSTRUCTOR_H_

#include <vector>

#include "parser.h"

class Constructor {
private:
  const NodeExit AST;
  size_t index{};

public:
  Constructor(std::optional<NodeExit> &AST) : AST{std::move(AST.value())} {}
  ~Constructor() = default;
};

#endif
