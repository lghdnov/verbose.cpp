cmake_minimum_required(VERSION 3.16)
project(verbose CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT MSVC)
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

add_library(verbalizer_core OBJECT
    src/verbalizer.cpp
    src/registry.cpp
)
target_include_directories(verbalizer_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(verbalizer_languages OBJECT
    src/languages/english.cpp
    src/languages/russian.cpp
)
target_link_libraries(verbalizer_languages PRIVATE verbalizer_core)

add_executable(verbose src/main.cpp src/cli.cpp)
target_link_libraries(verbose PRIVATE verbalizer_core verbalizer_languages)

add_executable(verbalizer_test tests/verbalizer_test.cpp)
target_link_libraries(verbalizer_test PRIVATE verbalizer_core verbalizer_languages)

enable_testing()
add_test(NAME verbalizer_test COMMAND verbalizer_test)
