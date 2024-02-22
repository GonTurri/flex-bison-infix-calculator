cflags = -g -Wall -pedantic-errors -D_GNU_SOURCE -std=c2x

calculadora : scanner.o parser.o main.o calculadora.o
	gcc scanner.o parser.o main.o calculadora.o -o calculadora -L/usr/lib/ -lfl -lm

parser.c parser.h : parser.y
	bison parser.y

parser.o : scanner.h parser.h parser.c
	gcc $(cflags) -c parser.c

main.o: main.c parser.h calculadora.h
	gcc $(cflags) -c main.c

calculadora.o: calculadora.c parser.h calculadora.h
	gcc $(cflags) -c calculadora.c

scanner.c scanner.h : scanner.l parser.h
	flex scanner.l

scanner.o : scanner.c scanner.h parser.h
	gcc $(cflags) -c scanner.c

clean : 
	rm scanner.c scanner.h scanner.o main.o parser.c parser.h parser.o calculadora.o calculadora