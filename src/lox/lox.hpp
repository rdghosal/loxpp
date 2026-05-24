#pragma once
#include <string_view>

class Lox {
  public:
    static void run(std::string_view source);
    static void run_file(std::string_view path);
    static void run_prompt();
    static void error(size_t line, std::string_view message);

  private:
    static bool had_error_;
    static void report(size_t line, std::string_view where, std::string_view message);
};
