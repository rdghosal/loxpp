#include "lox.hpp"
#include <fstream>
#include <iostream>
#include <print>
#include <sstream>
#include <string>

namespace lox {

void run(std::string_view source) {
    std::println("lox++ — running: {}", source);
}

void run_prompt() {
    std::string line{};
    while (std::print("> "), std::getline(std::cin, line)) {
        run(line);
    }
}

void run_file(std::string_view path) {
    std::ifstream file{std::string{path}};
    std::ostringstream buf;
    buf << file.rdbuf();
    run(buf.str());
}

} // namespace lox
