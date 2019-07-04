cmake_minimum_required(VERSION 3.14)
project(Compilatore C)

set(CMAKE_C_STANDARD 99)

add_executable(main lexer.c parser.c syntaxtree.c symboltable.c main.c)