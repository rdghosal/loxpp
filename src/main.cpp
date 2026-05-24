#include "lox/lox.hpp"
#include <print>
#include <span>
#include <string_view>

int main(int argc, char *argv[]) {
    auto args = std::span(argv, static_cast<std::size_t>(argc));

    switch (args.size()) {
    case 1:
        Lox::run_prompt();
        break;
    case 2:
        Lox::run_file(args[1]);
        break;
    default:
        std::println(stderr, "Usage: lox++ [script]");
        return 64;
    }
    return 0;
}
