#ifndef _PARSER_H_
#define _PARSER_H_

#include <cstdlib>
#include <optional>
#include <vector>

#include "tokenizer.h"

class NodeConstructor {};

class NodeBody : public NodeConstructor {
public:
  std::vector<NodeConstructor> values;
};

class NodeExpr : public NodeConstructor {
public:
  std::vector<Token> values;
};

class NodeExit : public NodeConstructor {
public:
  NodeExpr value;
};

class Parser {
private:
  const std::vector<Token> tokens;
  size_t index{};

  std::optional<Token> peek(size_t offset = 0) {
    if (tokens.size() > (index + offset)) {
      return tokens[index + offset];
    } else {
      return {};
    }
  }

  std::optional<Token> consume() {
    if (tokens.size() > index) {
      return tokens[index++];
    } else {
      return {};
    }
  }

public:
  Parser(std::vector<Token> tokens) : tokens{std::move(tokens)} {}
  ~Parser() = default;

  std::optional<NodeBody> parse() noexcept {
    NodeBody node_body;

    if (peek().has_value() && peek()->type == TokenType::Exit) {
      while (peek().has_value()) {
        NodeExit node_exit;
        consume();
        if (peek()->type == TokenType::ParenOpen) {
          consume();
          while (peek().has_value() && peek()->type == TokenType::IntLit) {
            node_exit.value.values.push_back(peek().value());
            consume();
          }
          if (peek()->type != TokenType::ParenClose) {
            std::cerr << "\nFalta fechar parenteses" << std::endl;
            exit(EXIT_FAILURE);
          }
        } else {
          std::cerr << "\nFalta abrir parenteses" << std::endl;
          exit(EXIT_FAILURE);
        }

        consume();
        if (node_exit.value.values.size() != 1) {
          std::cerr << "\nArgumentos incorretos!" << std::endl;
          exit(EXIT_FAILURE);
        }
        if (peek()->type != TokenType::SemiCo) {
          std::cerr << "\nFalta uso de \";\"" << std::endl;
          exit(EXIT_FAILURE);
        }
        node_body.values.push_back(node_exit);
        consume();
      }
      return node_body;
    } else {
      std::cerr << "\nMetodo não reconhecido!" << std::endl;
      exit(EXIT_FAILURE);
    }
    return {};
  }
};

#endif
