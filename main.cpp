#include <iostream>
#include <string>
#include <fstream>
#include <vector>

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



int main()
{
	string choice;
	cout <<"What would you like to do?"<<endl
		 <<"Type"<<endl
		 <<"\"1\"to create a table."<<endl
		 <<"\"2\" to transform an expression."<<endl;
	cin>>choice;
	while (choice !="1"&&choice !="2")
	{
		cout <<"Invalid choice. Please try again."<<endl
			 <<"Type"<<endl
			 <<"\"1\"to create a table."<<endl
			 <<"\"2\" to transform an expression."<<endl;
		cin >> choice;
	}
    cin.ignore();
	if (choice=="1")
	{
        // TODO: Finish integrating `export_table`
        Statement stm = prompt_expression();
        string filename;
        TableFormat table_fmt;

        cout << "Please enter the file to output to: ";
        getline(cin, filename);
        if (filename == "-") {
        } else if (filename.substr(filename.size()-4) == ".txt") {
            table_fmt = TXT;
        } else if (filename.substr(filename.size()-5) == ".html") {
            table_fmt = HTML;
        } else {
            do {
                cout << "Please select the output format:\n"
                     << "1. Plaintext\n"
                     << "2. HTML + CSS\n"
                     << ">> ";
                getline(cin, choice);
                switch (choice[0]) {
                    case '1': table_fmt = TXT; break;
                    case '2': table_fmt = HTML; break;
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
            }
            else
                cout << "Error opening \"" << filename << "\"\n";
        }
	}
	else
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
}

