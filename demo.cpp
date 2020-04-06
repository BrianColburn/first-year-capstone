#include "validate.hpp"
#include "statement.hpp"

int main() {
    std::string input;
    std::cout << "Please enter a logical expression: ";
    getline(std::cin, input);
    std::cout << "Received \"" << input << "\"\n";
    remove_spaces(input);
    std::cout << "After removing spaces: \"" << input << "\"\n";

    if (is_valid_statement(input)) {
        std::cout << "The expression was found valid.\n";

        std::string to_parse = add_parentheses(input);
        std::cout << "Adding parentheses for parsing: \"" << to_parse << "\"\n";

        Statement statement = parse_string(to_parse);
        std::cout << "Parsed statement: \"" << statement << "\"\n";

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
    } else {
        std::cout << "The expression was not found valid. There should be error messages printed above that can help.\n";
    }
    std::cout << "Exiting\n";
 
    return 0;
}
