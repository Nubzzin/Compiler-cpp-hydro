#ifndef _TOKENIZER_H_
#define _TOKENIZER_H_

#include <cctype>
#include <cstdlib>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

enum class TokenType {
  Exit,
  IntLit,
  SemiCo,
  ParenOpen,
  ParenClose,
};

struct Token {
  TokenType type;
  std::optional<std::string> value;
};

class Tokenizer {
private:
  const std::string src;
  size_t index{};

  std::optional<char> peek(size_t offset = 0) {
    if (src[index + offset]) {
      return src[index + offset];
    } else {
      return {};
    }
  }

  std::optional<char> consume() {
    if (!src[index++]) {
      return src[index++];
    } else {
      return {};
    }
  }

public:
  Tokenizer(std::string src) : src{src} {}
  ~Tokenizer() = default;

  std::vector<Token> tokenize() {
    std::vector<Token> tokens;
    std::string buf;

    while (peek().has_value()) {
      if (std::isalpha(peek().value())) {
        buf.push_back(peek().value());
        consume();
        while (std::isalnum(peek().value())) {
          buf.push_back(peek().value());
          consume();
        }

        if (buf == "exit") {
          tokens.push_back({.type = TokenType::Exit});
          buf.clear();
        }
      } else if (std::isdigit(peek().value())) {
        buf.push_back(peek().value());
        consume();
        while (std::isdigit(peek().value())) {
          buf.push_back(peek().value());
          consume();
        }
        tokens.push_back({.type = TokenType::IntLit, .value = buf});
        buf.clear();
      } else if (peek().value() == ';') {
        tokens.push_back({.type = TokenType::SemiCo});
        consume();
        buf.clear();
      } else if (std::isspace(peek().value())) {
        consume();
        continue;
      } else if (peek().value() == '(') {
        tokens.push_back({.type = TokenType::ParenOpen});
        consume();
        buf.clear();
      } else if (peek().value() == ')') {
        tokens.push_back({.type = TokenType::ParenClose});
        consume();
        buf.clear();
      } else {
        std::cerr << "Syntax error!" << std::endl;
        exit(EXIT_FAILURE);
      }
    }
    return tokens;
  }
};

#endif
