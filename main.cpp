#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <filesystem>

#include "validate.h"
#include "statement.h"
#include "table.h"
using namespace logicians;
using namespace std;


Statement prompt_expression() {
    string input;
    cout << "Please enter a logical expression: ";
    getline(cin, input);
    remove_spaces(input);

    while (!is_valid_statement(input)) {
        cout << "Please enter a logical expression: ";
        getline(cin, input);
        remove_spaces(input);
    }

    return parse_string(add_parentheses(input));
}

const string MENU = "1) Create a Truth Table\n"
                    "2) Transform an Expression\n"
                    "3) Help\n"
                    "4) About\n"
                    "5) Exit\n";


bool prompt_menu()
{
    string choice;

    cout << "What would you like to do?" << endl
         << "Type" << endl
         << MENU
         << ">> ";
    cin>>choice;
    while (choice != "1" &&
           choice != "2" &&
           choice != "3" &&
           choice != "4" &&
           choice != "5")
    {
        cout << "Invalid choice. Please try again." << endl
             << MENU
             << ">> ";
        cin >> choice;
    }
    cin.ignore();
    if (choice=="1")
    {
        // TODO: Finish integrating `export_table`
        Statement stm = prompt_expression();
        filesystem::path filename = filesystem::current_path();
        TableFormat table_fmt;

        cout << "Please enter the file to output to: ";
        getline(cin, choice);
        filename /= choice;

        if (choice == "-") {
        } else if (filename.extension() == ".txt") {
            table_fmt = TXT;
        } else if (filename.extension() == ".html") {
            table_fmt = HTML;
        } else {
            do {
                cout << "Please select the output format:\n"
                     << "1. Plaintext\n"
                     << "2. HTML + CSS\n"
                     << ">> ";
                getline(cin, choice);
                switch (choice[0]) {
                    case '1': table_fmt = TXT; filename += ".txt"; break;
                    case '2': table_fmt = HTML; filename += ".html"; break;
                    default: cout << "Invalid choice \"" << choice << "\"\n";
                }
            } while (choice[0] != '1' && choice[0] != '2');
        }

        //cout << "Using format " << table_fmt << "\n";

        if (filename == "-") {
                export_table(cout, TXT, stm, Statement::ASCII);
        } else {
            ofstream fout(filename);
            if (fout.is_open()) {
                export_table(fout, table_fmt, stm, Statement::ASCII);
                fout.close();
                cout << "The table was saved to "
                     << filename << endl
                     << "press any key to return to the menu...";
                getline(cin, choice);
            }
            else
                cout << "Error opening \"" << filename << "\"\n";
        }
    }

    else if (choice == "2")
    {

        Statement stm = prompt_expression();

        do {
            cout << "Please select from the following:\n"
                 << "1. Apply DeMorgan's Laws\n"
                 << "2. Cancel Double Negatives\n"
                 << "0. Exit\n"
                 << ">> ";
            getline(cin, choice);
            switch(choice[0]) {
                case '0': break;
                case '1': stm.transform(Statement::DeMORGANS); break;
                case '2': stm.transform(Statement::CANCEL_NOTS); break;
                default : cout << "Invalid choice\n";
            }
            if (choice[0] != '0')
                cout << "Expression is currently: \"" << stm.to_string() << "\"\n";
        } while (choice[0] != '0');
    }

    else if (choice == "3")
    {
        cout << "Logical expressions can be entered according to the following rules:\n"
             << "  A variable is any lowercase letter except 'v'\n"
             << "  Negation is denoted with '~', the tilde character\n"
             << "  Conjunction is denoted as \"p OP q\",\n"
             << "    where 'p' and 'q' are logical expressions,\n"
             << "    and \"OP\" is one of {\"AND\", '^', '&'}\n"
             << "  Disjunction is denoted as \"p OP q\",\n"
             << "    where 'p' and 'q' are logical expressions,\n"
             << "    and \"OP\" is one of {\"OR\", 'V', 'v', \"&&\"}\n"
             << "  Implication is denoted as \"p -> q\"\n"
             << "    where 'p' and 'q' are logical expressions,\n"
             << "  Equivalence is denoted as \"p OP q\",\n"
             << "    where 'p' and 'q' are logical expressions,\n"
             << "    and \"OP\" is one of {\"IFF\", \"<->\"}\n"
             << "\n"
             << "Expressions can be nested using parentheses.\n"
             << "Examples:\n"
             << "  p AND q\n"
             << "  pANDq\n"
             << "  (p)AND(q)\n"
             << "  p^q v r -> s <-> s v ~(r v q^p)\n"
             << "\n"
             << "press any key to return to the menu...";
        getline(cin, choice);
    }

    else if (choice == "4")
    {
        cout << "This program was commissioned by Dr. Matt Tedrow of Texas A&M University - Corpus Christi.\n"
             << "\n"
             << "The following students fulfilled the commission, as their First-Year Capstone Project, under the supervision of Dr. Rita Sperry.\n"
             << "Brandon Garcia\n"
             << "Brian Colburn\n"
             << "Mark Thompson\n"
             << "Xavier Linares\n"
             << "\n"
             << "\n"
             << "This application is copyrighted under the MIT license and the source code can be found on GitHub at\n"
             << "  https://github.com/BrianColburn/first-year-capstone"
             << "\n"
             << "press any key to return to the menu...";
        getline(cin, choice);
    }

    else if (choice == "5")
        return false;


    return true;
}

int main()
{
    while (prompt_menu());

    return 0;
}
