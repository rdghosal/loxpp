#pragma once
#include "token.hpp"
#include <string_view>
#include <unordered_map>
#include <vector>

class Scanner {
  public:
    Scanner(std::string_view src);
    std::vector<Token> scan_tokens();

  private:
    std::string_view source_;
    std::vector<Token> tokens_;
    std::size_t start_{0};
    std::size_t current_{0};
    std::size_t line_{1};
    static const std::unordered_map<std::string_view, TokenType> keywords_;
    bool is_at_end();
    void scan_token();
    char advance();
    void add_token(TokenType type);
    void add_token(TokenType type, Literal literal);
    char peek();
    char peek_next();
    bool match(char expected);
    void string();
    void number();
    void identifier();
    static bool is_digit(char c);
    static bool is_alpha(char c);
    static bool is_alphanumeric(char c);
};
