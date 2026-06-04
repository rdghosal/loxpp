#include "types.hpp"
#include "utils.hpp"
#include <print>
#include <span>

int main(int argc, char *argv[]) {
    auto args = std::span(argv, static_cast<std::size_t>(argc));

    if (args.size() != 2) {
        std::println(stderr, "Usage: gen-ast <output dir>");
        return static_cast<int>(ExitCode::USER_ERROR);
    }
    std::string_view output_dir{args[1]};
    std::println("recieved {}", output_dir);

    const std::vector<std::string_view> types = {
        "Binary   : Expr left, Token operator, Expr right", "Grouping : Expr expression",
        "Literal  : Object value", "Unary    : Token operator, Expr right"};
    define_ast(output_dir, types, "Expr");

    return 0;
}
