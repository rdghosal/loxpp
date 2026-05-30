#include "types.hpp"
#include <filesystem>
#include <fstream>
#include <print>
#include <ranges>
#include <string_view>
#include <vector>

// class Scanner {
//   public:
//     Scanner(std::string_view src);
//     std::vector<Token> scan_tokens();
//
//   private:
//     std::string_view source_;
// };

void define_visitor(std::ofstream &file, std::string_view base_name, std::string_view cls_name,
                    std::string_view fields) {

    // TODO: Templates?
    std::println(file, "class Visitor<T> {{");
}

void define_type(std::ofstream &file, std::string_view base_name, std::string_view cls_name,
                 std::string_view fields) {
    // Declaration
    std::println(file, "class {} : {} {{", cls_name, base_name);
    std::println(file, "  public:");
    std::println(file, "    {}({});", cls_name, fields);
    std::println(file, "  private:");
    for (const auto &f : std::views::split(fields, ', ')) {
        std::println(file, "    {}", f);
    }
    std::println(file, "}};");
}

void define_ast(std::string_view output_dir, std::string_view base_name,
                const std::vector<std::string_view> &types) {

    auto p_out_dir = std::filesystem::path{output_dir};
    if (!std::filesystem::is_directory(p_out_dir)) {
        std::println(stderr, "error: {} is not a directory", output_dir);
        return; // FIXME: should be handled with an ExitCode
    }
    auto p_out_file = p_out_dir / std::format("{}.cpp", base_name);
    std::ofstream file{p_out_file};
    if (!file) {
        std::println(stderr, "error: failed to create file {}", p_out_file.generic_string());
        return; // FIXME: should be handled with an ExitCode
    }

    // Visitor class
    // define_visitor();

    // Base class
    std::println(file, "class {} {{", base_name);
    std::println(file, "  virtual void something () const = 0;", base_name);
    std::println(file, "}};");

    // AST classes
    for (const auto &t : types) {
        auto parts = std::views::split(t, ':');
        auto it = parts.begin();
        std::string_view cls_name{*it};
        std::string_view fields{*++it};
        define_type(file, base_name, cls_name, fields);
    }
}
