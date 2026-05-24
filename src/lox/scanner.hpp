#pragma once
#include <string_view>

class Scanner {
  public:
    Scanner(std::string_view src);
    void scan_tokens();

  private:
    std::string_view source_;
};
