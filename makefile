compile:	
	flex -o lexer.c lexer.lex
	gcc -no-pie -g -o compile *.c
	  
