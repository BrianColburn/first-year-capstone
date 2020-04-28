default: main

STD ?= c++17

main: main.cpp validate.o statement.o table.o
	g++ -std=$(STD) main.cpp validate.o statement.o table.o -o logicians-main

tests_debug: tests.cpp validate.o statement.o
	g++ -std=$(STD) tests.cpp statement.o validate.o -o tests -DDEBUG_VALIDATE

table.o: table.cpp table.h
	g++ -std=$(STD) table.cpp -c

tests: tests.cpp validate.o statement.o
	g++ -std=$(STD) tests.cpp statement.o validate.o -o tests

statement.o: statement.cpp statement.h validate.o
	g++ -std=$(STD) statement.cpp -c

validate.o: validate.cpp validate.h
	g++ -std=$(STD) validate.cpp -c
