default: tests_debug

tests_debug: tests.cpp validate.o statement.o
	g++ tests.cpp statement.o validate.o -o tests -DDEBUG_VALIDATE

tests: tests.cpp validate.o statement.o
	g++ tests.cpp statement.o validate.o -o tests

statement.o: statement.cpp statement.h validate.o
	g++ statement.cpp -c

validate.o: validate.cpp validate.h
	g++ validate.cpp -c
