
add_executable(gen-ast gen_ast.cpp)
target_compile_options(gen-ast PRIVATE -Wall -Wextra -Wpedantic)
