CC = gcc
CFLAGS = -Wall -g

all: tarea1.exe

tarea1.exe: sat_main.o logic.o postfix_converter.o lex.yy.o
	$(CC) $(CFLAGS) -o tarea1.exe sat_main.o logic.o postfix_converter.o lex.yy.o

sat_main.o: sat_main.c logic.h postfix_converter.h
	$(CC) $(CFLAGS) -c sat_main.c

logic.o: logic.c logic.h
	$(CC) $(CFLAGS) -c logic.c

postfix_converter.o: postfix_converter.c postfix_converter.h
	$(CC) $(CFLAGS) -c postfix_converter.c

lex.yy.c: sat_lineal.l logic.h
	flex sat_lineal.l

lex.yy.o: lex.yy.c
	$(CC) $(CFLAGS) -c lex.yy.c

sat_solver.o: sat_solver.c logic.h
	$(CC) $(CFLAGS) -c sat_solver.c

clean:
	rm -f *.o lex.yy.c tarea1.exe