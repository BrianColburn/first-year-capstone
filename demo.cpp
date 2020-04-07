#include "validate.hpp"
#include "statement.hpp"

using namespace std;

int main() {
    string input;
    cout << "Please enter a logical expression: ";
    getline(cin, input);
    cout << "Received \"" << input << "\"\n";
    remove_spaces(input);
    cout << "After removing spaces: \"" << input << "\"\n";

    if (is_valid_statement(input)) {
        cout << "The expression was found valid.\n";

        string to_parse = add_parentheses(input);
        cout << "Adding parentheses for parsing: \"" << to_parse << "\"\n";

        Statement statement = parse_string(to_parse);
        cout << "Parsed statement: \"" << statement << "\"\n";

        set<char> vars = statement.collect_vars();
        cout << "\"" << statement << "\".collect_vars() = ";
        for (char c : vars)
            cout << c << ", ";
        cout << endl;

        auto subs = statement.collect_expressions();
        cout << "\"" << statement << "\".collect_expressions() = ";
        for (Statement s : subs)
            cout << s << ", ";
        cout << endl;

        auto variable_values = generate_vals(statement);
        for (map<char,bool> vars : variable_values) {
            cout << "For ";
            for (auto entry : vars) {
                cout << entry.first << "=" << entry.second << ", ";
            }
            cout << endl;
            cout << "\"" << statement << "\".evaluate(vars) = " << statement.evaluate(vars) << endl;
        }
    } else {
        cout << "The expression was not found valid. There should be error messages printed above that can help.\n";
    }
    cout << "Exiting\n";
 
    return 0;
}
