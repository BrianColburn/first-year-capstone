default: main

main: main.cpp validate.o statement.o table.o
	g++ -std=c++17 main.cpp validate.o statement.o table.o -o logicians-main

tests_debug: tests.cpp validate.o statement.o
	g++ -std=c++17 tests.cpp statement.o validate.o -o tests -DDEBUG_VALIDATE

table.o: table.cpp table.h
	g++ -std=c++17 table.cpp -c

tests: tests.cpp validate.o statement.o
	g++ -std=c++17 tests.cpp statement.o validate.o -o tests

statement.o: statement.cpp statement.h validate.o
	g++ -std=c++17 statement.cpp -c

validate.o: validate.cpp validate.h
	g++ -std=c++17 validate.cpp -c
