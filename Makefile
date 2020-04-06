default: tests_debug

demo:
	g++ demo.cpp -o demo

tests_debug: tests.cpp validate.hpp
	g++ tests.cpp -o tests -DDEBUG_VALIDATE

tests: tests.cpp validate.hpp
	g++ tests.cpp -o tests
