#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<optional>
#include<vector>

enum class TokenType {
  _return,
  int_lit,
  semi,
};

struct Token {
  TokenType type;
  std::optional<std::string>value;
};

std::vector<Token> tokenize(const std::string &str) {
  std::vector<Token>tokens{};
  std::string buf{};
  for (size_t i{0}; i < str.length(); i++) {
    char c = str.at(i);
    if (std::isalpha(c)) {
      buf.push_back(c);
      i++;
      while (std::isalnum(str.at(i))) {
        buf.push_back(str.at(i));
        i++;
      }
      i--;

      if (buf == "return") {
        tokens.push_back({.type = TokenType::_return});
        buf.clear();
        continue;
      }
      else {
        std::cerr << "You messed up!" << std::endl;
        exit(EXIT_FAILURE);
      }
    }
    else if (std::isdigit(c)) {
      buf.push_back(c);
      i++;
      while(std::isdigit(str.at(i))) {
        buf.push_back(str.at(i));
        i++;
      }
      i--;
      tokens.push_back({.type = TokenType::int_lit, .value = buf});
    }
    else if (c == ';') {
      tokens.push_back({.type = TokenType::semi});
    }
    else if (std::isspace(c)) {
      continue;
    }
    else {
      std::cerr << "You messed up! SyntaxError" << std::endl;
      exit(EXIT_FAILURE);
    }
  }
  return tokens;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Incorrect usage. Use:" << std::endl;
    std::cerr << "hydro <input.hy>" << std::endl;
    return EXIT_FAILURE;
  }

  std::string contents{};
  {
    std::stringstream contents_stream;
    std::fstream input(argv[1], std::ios::in);
    contents_stream << input.rdbuf();
    contents = contents_stream.str();
  }

  std::vector<Token>thing = tokenize(contents);

  std::cout << thing.size();

  return EXIT_SUCCESS;
}