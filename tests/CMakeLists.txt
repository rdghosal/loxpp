find_package(Catch2 3 REQUIRED)
include(CTest)
include(Catch)

add_executable(loxpp_tests lox_test.cpp)
target_link_libraries(loxpp_tests PRIVATE loxpp_lib Catch2::Catch2WithMain)
target_compile_options(loxpp_tests PRIVATE -Wall -Wextra -Wpedantic)

catch_discover_tests(loxpp_tests)
