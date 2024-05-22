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

struct NodeMainExpr {
  std::variant<NodeExit> main_expr_value;
};

struct NodeMain {
  std::vector<NodeMainExpr> main_values;
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
    // Buscando por main
    if (peek().has_value() && peek()->type == TokenType::Main) {
      NodeMain node_main;
      while (peek().has_value()) {
        consume();
        if (peek()->type == TokenType::CurlyOpen) {
          consume();
          NodeMainExpr node_main_expr;
          if (peek().has_value() && peek()->type == TokenType::Exit) {
            consume();
            NodeExit node_exit;
            if (peek()->type != TokenType::ParenOpen) {
              std::cerr << "\nFalta abrir parenteses" << std::endl;
              exit(EXIT_FAILURE);
            }
            consume();
            if (peek()->type == TokenType::IntLit) {
              node_exit.exit_value.exit_expr_value = peek().value();
            } else {
              std::cerr << "\nExit sem argumentos" << std::endl;
              exit(EXIT_FAILURE);
            }
            consume();
            if (peek()->type != TokenType::ParenClose) {
              std::cerr << "\nFalta fechar parenteses" << std::endl;
            }
            consume();
            if (peek()->type != TokenType::SemiCo) {
              std::cerr << "\nFalta uso de \";\"" << std::endl;
              exit(EXIT_FAILURE);
            }
            node_main_expr.main_expr_value = node_exit;
            node_main.main_values.push_back(node_main_expr);
          }
        } else if (peek(-2)->type == TokenType::CurlyClose) {
          std::cerr << "\nArgumentos pos Main function" << std::endl;
          exit(EXIT_FAILURE);
        } else {
          std::cerr << "\nFalta abrir Curly" << std::endl;
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
