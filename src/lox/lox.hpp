#pragma once
#include <string_view>

namespace lox {

void run(std::string_view source);
void run_file(std::string_view path);
void run_prompt();

} // namespace lox
