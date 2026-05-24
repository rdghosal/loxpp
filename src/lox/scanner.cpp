#include "scanner.hpp"
#include <print>

Scanner::Scanner(std::string_view src) : source_{src} {}

void Scanner::scan_tokens() {
    std::println("scanning source");
}
