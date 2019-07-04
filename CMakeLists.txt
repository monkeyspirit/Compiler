cmake_minimum_required(VERSION 3.14)
project(untitled C)

set(CMAKE_C_STANDARD 99)
set(SOURCE_FILES parser.c def.c)
set(SOURCE_FILES symbEsempio.c def.c)
add_executable(main main.c def.h syntaxTree.c lexer.c parser.c symboleTable.c)
add_executable(symbolTable def.h symbEsempio.c)