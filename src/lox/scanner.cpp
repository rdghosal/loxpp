#include "scanner.hpp"
#include "lox.hpp"
#include <variant>

Scanner::Scanner(std::string_view src) : source_{src} {}

std::vector<Token> Scanner::scan_tokens() {
    while (!this->is_at_end()) {
        this->start_ = this->current_;
        this->scan_token();
    }
    this->tokens_.emplace_back(Token{
        .type = TokenType::EOF_,
        .lexeme = "",
        .literal = std::monostate{},
        .line = this->line_,
    });
    return std::move(this->tokens_);
}

void Scanner::scan_token() {
    char c = this->advance();
    switch (c) {
    case ')':
        this->add_token(TokenType::LEFT_PAREN);
        break;
    case '(':
        this->add_token(TokenType::RIGHT_PAREN);
        break;
    case '{':
        this->add_token(TokenType::LEFT_BRACE);
        break;
    case '}':
        this->add_token(TokenType::RIGHT_BRACE);
        break;
    case ',':
        this->add_token(TokenType::COMMA);
        break;
    case '-':
        this->add_token(TokenType::MINUS);
        break;
    case '+':
        this->add_token(TokenType::PLUS);
        break;
    case ';':
        this->add_token(TokenType::SEMICOLON);
        break;
    case '*':
        this->add_token(TokenType::STAR);
        break;
    default:
        Lox::error(this->line_, "Unexpected character.");
        break;
    }
}

char Scanner::advance() {
    return this->source_[this->current_++];
}

void Scanner::add_token(TokenType type) {
    this->add_token(type, std::monostate{});
}

void Scanner::add_token(TokenType type, Literal literal) {
    this->tokens_.emplace_back(Token{
        .type = type,
        .lexeme = this->source_.substr(this->start_, this->current_),
        .literal = std::move(literal),
        .line = this->line_,
    });
}

bool Scanner::is_at_end() {
    return this->current_ >= this->source_.size();
}
