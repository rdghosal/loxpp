#pragma once
#include "token.hpp"
#include <string_view>
#include <vector>

class Scanner {
  public:
    Scanner(std::string_view src);
    std::vector<Token> &scan_tokens();

  private:
    std::string_view source_;
    std::vector<Token> tokens_;
    std::size_t start_;
    std::size_t current_;
    std::size_t line_;
    bool is_at_end();
    void scan_token();
    char advance();
    void add_token(TokenType type);
    void add_token(TokenType type, Literal literal);
};
