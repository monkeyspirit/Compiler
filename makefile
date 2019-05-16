all: tdl

tdl: lexer.o parser.o tree.o
	cc -no-pie -g -o tdl lexer.o parser.o tree.o

parser.o: parser.c def.h
	cc -no-pie -g -c parser.c

tree.o: tree.c def.h
	cc -no-pie -g -c tree.c

lexer.c: lexer.lex def.h
	flex -o lexer.c lexer.lex

lexer.o: lexer.c def.h
	cc -no-pie -g -c lexer.c




