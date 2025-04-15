CC = gcc
CFLAGS = -Wall -g

all: sat_cnf

sat_cnf: sat_main.o logic.o postfix_converter.o lex.yy.o
	$(CC) $(CFLAGS) -o sat_cnf sat_main.o logic.o postfix_converter.o lex.yy.o

sat_main.o: sat_main.c logic.h postfix_converter.h
	$(CC) $(CFLAGS) -c sat_main.c

logic.o: logic.c logic.h
	$(CC) $(CFLAGS) -c logic.c

postfix_converter.o: postfix_converter.c postfix_converter.h
	$(CC) $(CFLAGS) -c postfix_converter.c

lex.yy.c: sat_cnf.l logic.h
	flex sat_cnf.l

lex.yy.o: lex.yy.c
	$(CC) $(CFLAGS) -c lex.yy.c

clean:
	rm -f *.o lex.yy.c sat_cnf
