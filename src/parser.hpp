#ifndef _PARSER_H_
#define _PARSER_H_

#include <cstdlib>
#include <optional>
#include <variant>
#include <vector>

#include "tokenizer.hpp"

struct NodeExitExpr {
public:
  std::optional<Token> exit_expr_value;
};

struct NodeExit {
public:
  NodeExitExpr exit_value;
};

struct NodeMain {
  std::variant<NodeExit> main_value;
};

class Parser {
private:
  const std::vector<Token> tokens;
  size_t index{};

  // Lendo item no index atual
  std::optional<Token> peek(size_t offset = 0) {
    if (tokens.size() > (index + offset)) {
      return tokens[index + offset];
    } else {
      return {};
    }
  }

  // Lendo item no index atual e em seguido passando para o proximo
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

  std::optional<NodeMain> parse() noexcept {
    NodeMain node_main;

    // Buscando por main
    if (peek().has_value() && peek()->type == TokenType::Main) {
      while (peek().has_value()) {
        consume();
        if (peek()->type == TokenType::CurlyOpen) {
          consume();
          if (peek().has_value() && peek()->type == TokenType::Exit) {
            consume();
            if (peek()->type != TokenType::ParenOpen) {
              std::cerr << "\nFalta abrir parenteses" << std::endl;
              exit(EXIT_FAILURE);
            }
            consume();
            if (peek()->type == TokenType::IntLit) {
              std::get<NodeExit>(node_main.main_value)
                  .exit_value.exit_expr_value = peek().value();
            } else {
              std::cerr << "\nExit sem argumentos" << std::endl;
              exit(EXIT_FAILURE);
            }
            consume();
            if (peek()->type != TokenType::ParenClose) {
              std::cerr << "\nFalta fechar parenteses" << std::endl;
            }
          }
        } else if (peek(-2)->type == TokenType::CurlyClose) {
          std::cerr << "\nArgumentos pos Main function" << std::endl;
          exit(EXIT_FAILURE);
        } else {
          std::cerr << "\nFalta abrir Curly" << std::endl;
          exit(EXIT_FAILURE);
        }

        consume();
        if (!std::get<NodeExit>(node_main.main_value)
                 .exit_value.exit_expr_value.has_value()) {
          std::cerr << "\nArgumentos incorretos!" << std::endl;
          exit(EXIT_FAILURE);
        }
        if (peek()->type != TokenType::SemiCo) {
          std::cerr << "\nFalta uso de \";\"" << std::endl;
          exit(EXIT_FAILURE);
        }
        consume();
        if (peek()->type != TokenType::CurlyClose) {
          std::cerr << "\nFalta fechar Curly" << std::endl;
          exit(EXIT_FAILURE);
        }
        consume();
      }
      return node_main;
    } else {
      std::cerr << "\n\"Main\" não encontrado!" << std::endl;
      exit(EXIT_FAILURE);
    }
    return {};
  }
};

#endif
