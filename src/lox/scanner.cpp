#include "scanner.hpp"
#include "lox.hpp"
#include <print>
#include <unordered_map>
#include <variant>

Scanner::Scanner(std::string_view src) : source_{src} {}

const std::unordered_map<std::string_view, TokenType> Scanner::keywords_{
    {"and", TokenType::AND},     {"class", TokenType::CLASS},   {"else", TokenType::ELSE},
    {"false", TokenType::FALSE}, {"fun", TokenType::FUN},       {"for", TokenType::FOR},
    {"if", TokenType::IF},       {"nil", TokenType::NIL},       {"or", TokenType::OR},
    {"print", TokenType::PRINT}, {"return", TokenType::RETURN}, {"super", TokenType::SUPER},
    {"this", TokenType::THIS},   {"true", TokenType::TRUE},     {"var", TokenType::VAR},
    {"while", TokenType::WHILE}, {"eof_", TokenType::EOF_},
};

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
    case '/':
        if (match('/')) {
            // Handle comments
            while (peek() != '\n' && is_at_end()) {
                std::println("peeking...");
                advance();
            }
        } else {
            add_token(TokenType::SLASH);
        }
        break;
    case ' ':
    case '\r':
    case '\t':
        break;
    case '\n':
        line_++;
        break;
    case '"':
        string();
        break;
    default:
        if (is_digit(c)) {
            number();
        } else if (is_alpha(c)) {
            identifier();
        } else {
            Lox::error(line_, "Unexpected character.");
        }
        break;
    }
}

char Scanner::advance() {
    return source_[current_++];
}

void Scanner::string() {
    while (peek() != '"' && !is_at_end()) {
        if (peek() == '\n') {
            line_++;
        }
        advance();
    }
    if (is_at_end()) {
        Lox::error(line_, "Unterminated string.");
        return;
    }
    // Consume the closing quote
    advance();

    // Trim surroudning quotes
    std::string value{source_.substr(start_ + 1, current_ - 1)};
    add_token(TokenType::STRING, value);
}

void Scanner::number() {
    while (is_digit(peek())) {
        advance();
    }

    if (peek() == '.' && is_digit(peek_next())) {
        // Consume decimal
        advance();
    }
    while (is_digit(peek())) {
        advance();
    }

    // Trim surroudning quotes
    double value{
        std::stod(std::string{
            source_.substr(start_, current_ - start_),
        }),
    };
    add_token(TokenType::NUMBER, value);
}

void Scanner::identifier() {
    while (is_alphanumeric(peek())) {
        advance();
    }
    auto text = source_.substr(start_, current_ - start_);
    auto token_t = keywords_.find(text);
    if (token_t != keywords_.end()) {
        add_token(token_t->second);
    } else {
        add_token(TokenType::IDENTIFIER);
    }
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

char Scanner::peek() {
    return (is_at_end()) ? '\0' : source_.at(current_);
}

char Scanner::peek_next() {
    return (current_ + 1 >= source_.size()) ? '\0' : source_.at(current_ + 1);
}

bool Scanner::is_digit(char c) {
    return c >= '0' && c <= '9';
}

bool Scanner::is_alpha(char c) {
    return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c == '_'));
}

bool Scanner::is_alphanumeric(char c) {
    return is_digit(c) || is_alpha(c);
}

bool Scanner::is_at_end() {

    return current_ >= source_.size();
}
