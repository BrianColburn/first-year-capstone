#include "validate.hpp"
#include "statement.hpp"

std::string test_valid_stm(std::string stm) {
    std::cout << "Testing statement \"" << stm << "\"\n";
    if (is_valid_statement(stm) > 0)
        return stm + " is a valid statement\n";
    else
        return stm + " is NOT a valid statement\n";
}

int main(int argc, char const *argv[])
{
    std::cout << "Testing parentheses functions:\n";
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

    std::cout << "\n\n";

    /*std::cout << "Testing `is_valid_statement':\n";
    std::string stm = "(pvr)^(~q)&r";
    std::cout << test_valid_stm(stm) << std::endl;
    stm = "(pvr)&(~q)";
    std::cout << test_valid_stm(stm) << std::endl;
    stm = "(pvr)AND(~q)";
    std::cout << test_valid_stm(stm) << std::endl;
    stm = "(pvr)^~q";
    std::cout << test_valid_stm(stm) << std::endl;
    stm = "(pvr)^(~)";
    std::cout << test_valid_stm(stm) << std::endl;
    stm = "";
    std::cout << test_valid_stm(stm) << std::endl;
    stm = "()";
    std::cout << test_valid_stm(stm) << std::endl;
    stm = "~~(~~~p)";
    std::cout << test_valid_stm(stm) << std::endl;
    stm = "pORqV(r->s^p<->r->~(~p&&(~(s))))AND(pORqIFFqVp)&((~~r))";
    std::cout << test_valid_stm(stm) << std::endl;
    stm = "(a)";
    std::cout << test_valid_stm(stm) << std::endl;
    stm = "(a)b";
    std::cout << test_valid_stm(stm) << std::endl;
    stm = "eeee";
    std::cout << test_valid_stm(stm) << std::endl;
    stm = "<->(->(OR(AND(~b))))";
    std::cout << test_valid_stm(stm) << std::endl;
    stm = "e";
    std::cout << test_valid_stm(stm) << std::endl;
    stm = "(abcd)v(efgh)&i";
    std::cout << test_valid_stm(stm) << std::endl;

    std::cout << "\n";*/

    std::cout << "Testing `add_parentheses':\n";
    std::string stm = "~a^bVc->d<->e->fORgAND~h";
    std::string res = add_parentheses(stm);
    std::cout << "add_parentheses(\"" << stm << "\") = \"" << res << "\"\n";
    std::vector<std::string> operands = find_operands(res,0);
    //for (auto s : operands) {
    //    std::cout << s << std::endl;
    //}
    //std::cout << parse_vector(operands) << std::endl;
    std::cout << "parse_string(\"" << res << "\") = " << parse_string(res) << std::endl;
    //std::cout << parse_string("(a)^(b)") << std::endl;
    stm = "~~(~~~p)";
    res = add_parentheses(stm);
    std::cout << "add_parentheses(\"" << stm << "\") = \"" << res << "\"\n";
    std::cout << "parse_string(\"" << res << "\") = " << parse_string(res) << std::endl;
    stm = "aANDb&&c";
    res = add_parentheses(stm);
    std::cout << "add_parentheses(\"" << stm << "\") = \"" << res << "\"\n";
    std::cout << "parse_string(\"" << res << "\") = " << parse_string(res) << std::endl;
    stm = "pORqV(r->s^p<->r->~(~p&&(~(s))))AND(pORqIFFqVp)&((~~r))";
    res = add_parentheses(stm);
    std::cout << "add_parentheses(\"" << stm << "\") = \"" << res << "\"\n";
    std::cout << "parse_string(\"" << res << "\") = " << parse_string(res) << std::endl;
    return 0;
}
