cmake_minimum_required(VERSION 3.5.1)
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

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -g -pthread")

add_subdirectory(lib/splittercell)

include_directories(include)
include_directories(lib_headers)
include_directories(lib/splittercell/include)

set(SOURCE_FILES src/main.cpp src/graph.cpp src/argument.cpp src/tree.cpp)
add_executable(stardec ${SOURCE_FILES} ${FLEX_BISON_FILES})
target_link_libraries(stardec splittercell)
