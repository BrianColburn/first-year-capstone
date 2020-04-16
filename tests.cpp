#include "validate.h"
#include "statement.h"

using namespace logicians;

std::string test_valid_stm(std::string stm) {
    std::cout << "Testing statement \"" << stm << "\"\n";
    if (is_valid_statement(stm))
        return stm + " is a valid statement\n";
    else
        return stm + " is NOT a valid statement\n";
}

void test_statements(std::string stm) {
    std::cout << "remove_spaces(\"" << stm << "\") = ";
    remove_spaces(stm);
    std::cout << '"' << stm << "\"\n";
    std::string res = add_parentheses(stm);
    std::cout << "add_parentheses(\"" << stm << "\") = \"" << res << "\"\n";

    Statement statement = parse_string(res);
    std::cout << "parse_string(\"" << res << "\") = " << statement << std::endl;

    int size = 0;
    std::vector<Statement> subs = statement.collect_expressions();
    std::cout << "\"" << statement << "\".collect_expressions() = ";
    for (Statement s : subs) {
        std::cout << s << ", ";
        size += sizeof(s);
    }
    std::cout << std::endl;

    subs = statement.collect_expressions_ordered();
    std::cout << "\"" << statement << "\".collect_expressions_ordered() = ";
    for (Statement s : subs)
        std::cout << s << ", ";
    std::cout << std::endl;
    std::cout << "Statement size: " << size << " bytes\n";
    statement.transform(DeMORGANS);
    std::cout << "DeMorgan'd: \"" << statement << "\"\n";
    statement.transform(CANCEL_NOTS);
    std::cout << "Simplified: \"" << statement << "\"\n";
    std::cout << "\n\n";
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

    std::cout << "Testing Statement Functions/Methods:\n";
    stm = "a AND b OR c";//"~a^bVc->d<->e->fORgAND~h";
    test_statements(stm);
    
    //std::cout << parse_string("(a)^(b)") << std::endl;
    stm = "~~(~~~p)";
    test_statements(stm);

    stm = "aANDb&&c";
    test_statements(stm);

    stm = "pORqV(r->s^p<->r->~(~p&&(~(s))))AND(pORqIFFqVp)&((~~r))";
    test_statements(stm);

    stm = "pANDq->z";
    test_statements(stm);

    stm = "a^~av(b->~a)";
    test_statements(stm);

    stm = "a";
    test_statements(stm);

    stm = "~a";
    test_statements(stm);

    stm = "avb";
    test_statements(stm);

    stm = "~(avb)";
    test_statements(stm);

    stm = "~~(avb)";
    test_statements(stm);

    stm = "~(avb)^c";
    test_statements(stm);
    return 0;
}
