# makefile  --  Used to make CS 375 compiler in C      21 Sep 04
#
# Gordon S. Novak Jr.
#
# token.h is assumed to be a local file because on some machines
# or yacc versions tokenb.h must be used instead (renamed token.h).

# To compile starter file lex1.c --> lex1
# 
lex1:   lexandr.o lex1.o scanner.o printtoken.o token.h lexan.h
	cc -o lex1 -lm lexandr.o lex1.o scanner.o printtoken.o