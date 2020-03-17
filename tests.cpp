#include "validate.hpp"

int main(int argc, char const *argv[])
{
    std::cout << (find_close_paren("f(x+1)",1) == 3) << std::endl;
    std::cout << (find_open_paren("f(x+1)",5) == 3) << std::endl;
    std::cout << parens_matched("f(x+1)") << std::endl;
    std::cout << !parens_matched("f)x+1(") << std::endl;
    std::cout << !parens_matched("f(x+1(") << std::endl;
    std::cout << !parens_matched("f)x+1)") << std::endl;

    std::string test_string = "f(x+g(y-h(1)))";
    int ix = 1;
    std::cout << test_string.substr(ix+1, find_close_paren(test_string, ix)) << std::endl;
    ix = 12;
    int length = find_open_paren(test_string, ix),
        start  = ix-length;
    std::cout << test_string.substr(start, length) << std::endl;

    std::string stm = "(pvr)^(!q)";
    std::cout << is_valid_statement(stm, 0, stm.size()) << std::endl;
    stm = "(pvr)^(!)";
    std::cout << !is_valid_statement(stm, 0, stm.size()) << std::endl;
    return 0;
}
