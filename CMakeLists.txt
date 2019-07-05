cmake_minimum_required(VERSION 3.14)
project(untitled C)

set(CMAKE_C_STANDARD 99)

add_executable(main main.c syntaxTree.c lexer.c parser.c symbolTable.c)
