cmake_minimum_required(VERSION 3.6)
project(stardec)

find_package(BISON)
find_package(FLEX)

IF(BISON_FOUND AND FLEX_FOUND)
    BISON_TARGET(parser parser/parser.ypp src/parser.cpp DEFINES_FILE include/parser.hpp)
    FLEX_TARGET(lexer parser/lexer.l src/lexer.cpp)
    ADD_FLEX_BISON_DEPENDENCY(lexer parser)
    set(FLEX_BISON_FILES ${BISON_parser_OUTPUTS} ${FLEX_lexer_OUTPUTS})
ELSE()
    set(FLEX_BISON_FILES src/parser.cpp src/lexer.cpp)
ENDIF()

add_subdirectory(lib/splittercell)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -O3 -g -pthread -pedantic -Wall")

include_directories(include)
include_directories(lib_headers)
include_directories(lib/splittercell/include)

set(SOURCE_FILES src/graph.cpp src/argument.cpp src/tree.cpp src/evaluator.cpp ${FLEX_BISON_FILES})
add_library(stardec ${SOURCE_FILES})
add_executable(stardecbin main.cpp)
target_link_libraries(stardecbin splittercell stardec)
