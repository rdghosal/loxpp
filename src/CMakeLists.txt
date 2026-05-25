add_library(loxpp_lib STATIC
  lox/lox.cpp
  lox/scanner.cpp
)
target_include_directories(loxpp_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_options(loxpp_lib PRIVATE -Wall -Wextra -Wpedantic -Werror)
target_compile_features(loxpp_lib PUBLIC cxx_std_23)

add_executable(loxpp main.cpp)
target_link_libraries(loxpp PRIVATE loxpp_lib)
target_compile_options(loxpp PRIVATE -Wall -Wextra -Wpedantic -Werror)
