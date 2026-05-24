#include "scanner.hpp"
#include "lox.hpp"
#include <variant>

Scanner::Scanner(std::string_view src) : source_{src} {}

std::vector<Token> Scanner::scan_tokens() {
    while (!is_at_end()) {
        start_ = current_;
        scan_token();
    }
    tokens_.emplace_back(Token{
        .type = TokenType::EOF_,
        .lexeme = "",
        .literal = std::monostate{},
        .line = line_,
    });
    return std::move(tokens_);
}

void Scanner::scan_token() {
    char c = advance();
    switch (c) {
    case ')':
        add_token(TokenType::LEFT_PAREN);
        break;
    case '(':
        add_token(TokenType::RIGHT_PAREN);
        break;
    case '{':
        add_token(TokenType::LEFT_BRACE);
        break;
    case '}':
        add_token(TokenType::RIGHT_BRACE);
        break;
    case ',':
        add_token(TokenType::COMMA);
        break;
    case '-':
        add_token(TokenType::MINUS);
        break;
    case '+':
        add_token(TokenType::PLUS);
        break;
    case ';':
        add_token(TokenType::SEMICOLON);
        break;
    case '*':
        add_token(TokenType::STAR);
        break;
    case '!':
        add_token(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
        break;
    case '=':
        add_token(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
        break;
    case '<':
        add_token(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
        break;
    case '>':
        add_token(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
        break;
    default:
        Lox::error(line_, "Unexpected character.");
        break;
    }
}

char Scanner::advance() {
    return source_[current_++];
}

void Scanner::add_token(TokenType type) {
    add_token(type, std::monostate{});
}

bool Scanner::match(char expected) {
    if (is_at_end()) {
        return false;
    }
    if (source_.at(current_) != expected) {
        return false;
    }
    current_++;
    return true;
}

void Scanner::add_token(TokenType type, Literal literal) {
    tokens_.emplace_back(Token{
        .type = type,
        .lexeme = source_.substr(start_, current_),
        .literal = std::move(literal),
        .line = line_,
    });
}

bool Scanner::is_at_end() {
    return current_ >= source_.size();
}
