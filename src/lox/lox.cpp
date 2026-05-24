#include "lox.hpp"
#include "scanner.hpp"
#include <fstream>
#include <iostream>
#include <print>
#include <sstream>
#include <string>

bool Lox::had_error_{false};

void Lox::run(std::string_view source) {
    Scanner scanner{source};
    scanner.scan_tokens();
}

void Lox::run_prompt() {
    std::string line;
    while (std::print("> "), std::getline(std::cin, line)) {
        run(line);
    }
}

void Lox::run_file(std::string_view path) {
    std::ifstream file{std::string{path}};
    if (!file) {
        std::println(stderr, "could not open file: {}", path);
        return;
    }
    std::ostringstream buf;
    buf << file.rdbuf();
    run(buf.str());
}

void Lox::error(std::size_t line, std::string_view message) {
    Lox::report(line, "", message);
}

void Lox::report(std::size_t line, std::string_view where, std::string_view message) {
    std::println("[line {}] Error {}: {}", line, where, message);
    had_error_ = true;
}
