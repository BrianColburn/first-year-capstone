#include "validate.hpp"
#include "statement.hpp"

std::string test_valid_stm(std::string stm) {
    std::cout << "Testing statement \"" << stm << "\"\n";
    if (is_valid_statement(stm))
        return stm + " is a valid statement\n";
    else
        return stm + " is NOT a valid statement\n";
}

int main(int argc, char const *argv[])
{
    std::string test_string = "f(x+g(y-h(1)))";
    int ix = 1;
    std::cout << test_string.substr(ix+1, find_close_paren(test_string, ix)) << std::endl;
    ix = 12;
    int length = find_open_paren(test_string, ix),
        start  = ix-length;
    std::cout << test_string.substr(start, length) << std::endl;

    std::cout << "\n\n";
    std::string stm;

    std::cout << "Testing `is_valid_statement':\n";
    stm = "(pvr)^(~q)&r";
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

    std::cout << "\n";

    std::cout << "Testing `add_parentheses':\n";
    stm = "a AND b OR c";//"~a^bVc->d<->e->fORgAND~h";
    std::cout << "remove_spaces(\"" << stm << "\") = ";
    remove_spaces(stm);
    std::cout << '"' << stm << "\"\n";
    std::string res = add_parentheses(stm);
    std::cout << "add_parentheses(\"" << stm << "\") = \"" << res << "\"\n";
    std::vector<std::string> operands = find_operands(res,0);
    //for (auto s : operands) {
    //    std::cout << s << std::endl;
    //}
    //std::cout << parse_vector(operands) << std::endl;
    Statement statement = parse_string(res);
    std::cout << "parse_string(\"" << res << "\") = " << statement << std::endl;
    std::set<char> vars = statement.collect_vars();
    std::cout << "\"" << statement << "\".collect_vars() = ";
    for (char c : vars)
        std::cout << c << ", ";
    std::cout << std::endl;
    auto subs = statement.collect_expressions();
    std::cout << "\"" << statement << "\".collect_expressions() = ";
    for (Statement s : subs)
        std::cout << s << ", ";
    std::cout << std::endl;
    auto variable_values = generate_vals(statement);
    for (std::map<char,bool> vars : variable_values) {
        std::cout << "For ";
        for (auto entry : vars) {
            std::cout << entry.first << "=" << entry.second << ", ";
        }
        std::cout << std::endl;
        std::cout << "\"" << statement << "\".evaluate(vars) = " << statement.evaluate(vars) << std::endl;
    }
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
