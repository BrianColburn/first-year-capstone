#include "validate.h"
#include "statement.h"

using namespace std;
using namespace logicians;

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
        cout << statement.collect_expressions().size() << endl;

        auto variable_values = generate_vals(statement);
        //for (map<char,bool> vars : variable_values) {
        for (int i = 0; i < variable_values.size(); i++) {
            vector<bool> values;
            map<char, bool> vars = variable_values[i];
            for (Statement s : subs)
                cout << s << ", ";
            cout << endl;
            for (Statement s : subs)
                values.push_back(s.evaluate(vars));
            cout << "Stored in vector: ";
            for (bool b : values)
                cout << b << ", ";
            cout << endl;
        }
    } else {
        cout << "The expression was not found valid. There should be error messages printed above that can help.\n";
    }
    cout << "Exiting\n";
 
    return 0;
}
