default: tests_debug

tests_debug: tests.cpp validate.hpp statement.hpp
	g++ tests.cpp -o tests -DDEBUG_VALIDATE

tests: tests.cpp validate.hpp statement.hpp
	g++ tests.cpp -o tests
