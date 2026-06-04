#include "types.hpp"
#include <filesystem>
#include <format>
#include <fstream>
#include <print>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>

// class Scanner {
//   public:
//     Scanner();
//     std::vector<Token> scan_tokens();
//
//   private:
//     std::string_view source_;
// };

auto rtrim(std::string_view &s) -> std::string_view {
    if (!s.contains(' ')) {
        return s;
    }
    s.remove_suffix(s.size() - s.find_first_of(' '));
    return s;
}

auto lower(std::string_view &sv) -> std::string {
    return sv | std::views::transform([](unsigned char c) {
               return static_cast<char>(std::tolower(c));
           }) |
           std::ranges::to<std::string>();
}

auto format(std::string_view type_name) -> std::string {
    type_name = rtrim(type_name);
    return lower(type_name);
}

void define_visitor(std::ofstream &file, std::string_view base_name,
                    const std::vector<std::string_view> &types) {
    std::println(file, "class Visitor {{");
    for (const auto &t : types) {
        auto parts = std::views::split(t, ':');
        auto it = parts.begin();
        std::string_view type_name{*it};
        std::println("{}", type_name);

        std::println(file, "virtual T visit_{}_{}({} {}) const = 0;", format(type_name),
                     format(base_name), rtrim(type_name), format(base_name));
    }
    std::println(file, "}};");
}

void define_type(std::ofstream &file, std::string_view base_name, std::string_view cls_name,
                 std::string_view fields) {
    // Declaration
    std::println(file, "class {} : {} {{", cls_name, base_name);
    std::println(file, "  public:");
    std::println(file, "    {}({});", cls_name, fields);
    std::println(file, "  private:");
    for (const auto &f : std::views::split(fields, ", ")) {
        std::println(file, "    {}", f);
    }
    std::println(file, "}};");
}

void define_ast(std::string_view output_dir, const std::vector<std::string_view> &types,
                std::string_view base_name) {

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

    // Declare Visitor class
    std::println(file, "template <typename T> class Visitor{{}};");

    // Base class
    std::println(file, "class {} {{", base_name);
    std::println(file, "  template <typename T> virtual accept(Visitor<T> visitor) const = 0;",
                 base_name);
    std::println(file, "}};");

    // AST classes
    for (const auto &t : types) {
        auto parts = std::views::split(t, ':');
        auto it = parts.begin();
        std::string_view cls_name{*it};
        std::string_view fields{*++it};
        define_type(file, base_name, cls_name, fields);
        std::println(file, "  template <typename T> accept(Visitor<T> visitor) override {{");
        std::println(file, "    visitor.visit_{}_{}(this);", cls_name, base_name);
        std::println(file, "  }}");
    }

    // Visitor class
    define_visitor(file, base_name, types);
}
