#ifndef _PARSER_H_
#define _PARSER_H_

#include <cstdlib>
#include <optional>
#include <variant>
#include <vector>

#include "tokenizer.hpp"

// Print
struct NodePrintExpr {
  std::optional<Token> expr_value;
};

struct NodePrint {
  NodePrintExpr print_value;
};

// Exit
struct NodeExitExpr {
  std::optional<Token> expr_value;
};

struct NodeExit {
  NodeExitExpr exit_value;
};

// Main
struct NodeMainExpr {
  std::variant<NodeExit, NodePrint> expr_value;
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
      consume();
      if (peek()->type == TokenType::CurlyOpen) {
        while (peek().has_value()) {
          consume();
          NodeMainExpr node_main_expr;
          if (peek()->type == TokenType::Exit) {
            NodeExit node_exit;
            consume();
            if (peek()->type != TokenType::ParenOpen) {
              std::cerr << "\nFalta abrir parenteses em \"Exit\"" << std::endl;
              exit(EXIT_FAILURE);
            }
            consume();
            if (peek()->type == TokenType::IntLit) {
              node_exit.exit_value.expr_value = peek().value();
              consume();
            } else {
              std::cerr << "\nFalta argumentos em \"Exit\"" << std::endl;
              exit(EXIT_FAILURE);
            }
            if (peek()->type != TokenType::ParenClose) {
              std::cerr << "\nFalta fechar parenteses em \"Exit\"" << std::endl;
            }
            consume();
            if (peek()->type != TokenType::SemiCo) {
              std::cerr << "\nFalta uso de \";\" em \"Exit\"" << std::endl;
              exit(EXIT_FAILURE);
            }

            node_main_expr.expr_value = node_exit;
            node_main.main_values.push_back(node_main_expr);
            continue;
          }
          if (peek()->type == TokenType::Print) {
            NodePrint node_print;
            consume();
            if (peek()->type != TokenType::ParenOpen) {
              std::cerr << "\nFalta abrir parenteses em \"Print\"" << std::endl;
              exit(EXIT_FAILURE);
            }
            consume();
            if (peek()->type == TokenType::IntLit ||
                peek()->type == TokenType::Str) {
              node_print.print_value.expr_value = peek().value();
            } else {
              std::cerr << "\nFalta argumentos em \"Print\"" << std::endl;
              exit(EXIT_FAILURE);
            }
            consume();
            if (peek()->type != TokenType::ParenClose) {
              std::cerr << "\nFalta fechar parentesesem em \"Print\""
                        << std::endl;
              exit(EXIT_FAILURE);
            }
            consume();
            if (peek()->type != TokenType::SemiCo) {
              std::cerr << "\nFalta uso de \";\" em \"Print\"" << std::endl;
              exit(EXIT_FAILURE);
            }
            node_main_expr.expr_value = node_print;
            node_main.main_values.push_back(node_main_expr);
            continue;
          }
          if (peek()->type != TokenType::CurlyClose) {
            std::cerr << "\nFalta fechar Curly" << std::endl;
            exit(EXIT_FAILURE);
          }
        }
      } else {
        std::cerr << "\nFalta abrir Curly" << std::endl;
        exit(EXIT_FAILURE);
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
