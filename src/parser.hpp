#ifndef _PARSER_H_
#define _PARSER_H_

#include <cstdlib>
#include <optional>
#include <variant>
#include <vector>

#include "tokenizer.hpp"

class NodeExitExpr {
public:
  std::optional<Token> exitExprvalue;
};

class NodeExit {
public:
  std::variant<NodeExitExpr> exitvalue;
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

  std::optional<NodeExit> parse() noexcept {
    NodeExit nodeExit;

    if (peek().has_value() && peek()->type == TokenType::Exit) {
      while (peek().has_value()) {
        consume();
        if (peek()->type == TokenType::ParenOpen) {
          consume();
          while (peek().has_value() && peek()->type == TokenType::IntLit) {
            std::get<NodeExitExpr>(nodeExit.exitvalue).exitExprvalue =
                peek().value();
            consume();
          }
          if (peek()->type != TokenType::ParenClose) {
            std::cerr << "Falta fechar parenteses" << std::endl;
            exit(EXIT_FAILURE);
          }
        } else {
          std::cerr << "Falta abrir parenteses" << std::endl;
          exit(EXIT_FAILURE);
        }

        consume();
        if (!std::get<NodeExitExpr>(nodeExit.exitvalue)
                 .exitExprvalue.has_value()) {
          std::cerr << "\nArgumentos incorretos!" << std::endl;
          exit(EXIT_FAILURE);
        }
        if (peek()->type != TokenType::SemiCo) {
          std::cerr << "\nFalta uso de \";\"" << std::endl;
          exit(EXIT_FAILURE);
        }
        consume();
      }
      return nodeExit;
    } else {
      std::cerr << "\nMetodo não reconhecido!" << std::endl;
      exit(EXIT_FAILURE);
    }
    return {};
  }
};

#endif
