CC = gcc
LEX = flex
BISON = bison
CFLAGS = -g -Wall

SRC = main.c ast.c symtab.c icg.c

all: compiler.exe

compiler.exe: lex.yy.c parser.tab.c $(SRC)
	$(CC) $(CFLAGS) -o compiler.exe lex.yy.c parser.tab.c $(SRC) -lm

lex.yy.c: lexer.l parser.tab.h
	$(LEX) lexer.l

parser.tab.c parser.tab.h: parser.y
	$(BISON) -d -v parser.y

clean:
	del /Q compiler.exe *.o lex.yy.c parser.tab.* parser.output 2>nul || exit 0
