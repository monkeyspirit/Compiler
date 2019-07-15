compile: main.o parser.o symbolTable.o syntaxTree.o dynamicArray.o semantic.o lexer.o tcode.o 
	cc -no-pie -g -o compile main.o lexer.o parser.o symbolTable.o dynamicArray.o syntaxTree.o tcode.o semantic.o	

main.o: main.c def.h parser.h syntaxTree.h symbolTable.h semantic.h tcode.h
	cc -no-pie -g -c main.c 

dynamicArray.o: dynamicArray.c dynamicArray.h
	cc -no-pie -g -c dynamicArray.c

tcode.o: tcode.c tcode.h semantic.h dynamicArray.h 
	cc -no-pie -g -c tcode.c

semantic.o: semantic.c semantic.h symbolTable.h
	cc -no-pie -g -c semantic.c

symbolTable.o: symbolTable.c symbolTable.h semantic.h
	cc -no-pie -g -c symbolTable.c

syntaxTree.o: syntaxTree.c syntaxTree.h semantic.h
	cc -no-pie -g -c syntaxTree.c

parser.o: parser.c def.h parser.h semantic.h
	cc -no-pie -g -c parser.c 

lexer.o: lexer.c def.h 
	cc -no-pie -g -c lexer.c

lexer.c: lexer.lex def.h
	flex -o lexer.c lexer.lex
