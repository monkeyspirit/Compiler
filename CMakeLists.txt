cmake_minimum_required(VERSION 3.14)
project(Compiler C)

set(CMAKE_C_STANDARD 99)

add_executable(main main.c semantic.c syntaxTree.c lexer.c parser.c symbolTable.c tcode.c tcode.h  dynamicArray.c dynamicArray.h)
