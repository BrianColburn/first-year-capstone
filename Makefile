default: demo

demo: demo.cpp validate.o statement.o
	g++ --std=c++11 demo.cpp -o demo

tests_debug: tests.cpp validate.o statement.o
	g++ --std=c++11 tests.cpp statement.o validate.o -o tests -DDEBUG_VALIDATE

tests: tests.cpp validate.o statement.o
	g++ --std=c++11 tests.cpp statement.o validate.o -o tests

statement.o: statement.cpp statement.h validate.o
	g++ --std=c++11 statement.cpp -c

validate.o: validate.cpp validate.h
	g++ --std=c++11 validate.cpp -c
