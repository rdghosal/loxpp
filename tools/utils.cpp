#include "types.hpp"
#include <filesystem>
#include <fstream>
#include <print>
#include <ranges>
#include <string_view>
#include <vector>

void define_type(std::ofstream &file, std::string_view base_name, std::string_view cls_name,
                 std::string_view fields) {}

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
    std::println(file, "class {} {{", base_name);
    std::println(file, "}};");

    for (auto &t : types) {
        auto parts = std::views::split(t, ':');
        auto it = parts.begin();
        std::string_view cls_name{*it};
        std::string_view fields{*++it};
        define_type(file, base_name, cls_name, fields);
    }
}
