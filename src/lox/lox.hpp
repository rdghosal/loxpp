#pragma once
#include <string_view>

enum class ExitCode : int { // NOLINT
    OK = 0,
    IO_ERROR = 63,
    RUNTIME_ERROR = 65,
};

class Lox {
  public:
    static void run(std::string_view source);
    static int run_file(std::string_view path);
    static void run_prompt();
    static void error(std::size_t line, std::string_view message);

  private:
    static bool had_error_;
    static void report(std::size_t line, std::string_view where, std::string_view message);
};
