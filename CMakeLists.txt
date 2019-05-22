cmake_minimum_required(VERSION 3.14)
project(untitled C)

set(CMAKE_C_STANDARD 99)
set(SOURCE_FILES parser.cpp def.cpp)
set(SOURCE_FILES symb.cpp def.cpp)
add_executable(syntaxTree def.h tree.c lexer.c parser.c)
add_executable(symbolTable def.h symb.c)