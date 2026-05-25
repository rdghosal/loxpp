cmake_minimum_required(VERSION 3.28)

project(loxpp
  VERSION 0.1.0
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(ENABLE_CLANG_TIDY "Enable clang-tidy static analysis" OFF)
if(ENABLE_CLANG_TIDY)
  find_program(CLANG_TIDY_EXE NAMES clang-tidy REQUIRED)
  set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")
endif()

option(ENABLE_SANITIZERS "Enable AddressSanitizer and UndefinedBehaviorSanitizer" OFF)
if(ENABLE_SANITIZERS)
  add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
  add_link_options(-fsanitize=address,undefined)
endif()

enable_testing()

add_subdirectory(src)
add_subdirectory(tools)
add_subdirectory(tests)
