#include <iostream>
#include <string>

#include "validate.h"
#include "statement.h"
using namespace logicians;
using namespace std;

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
        cout << "Truth-Table generation is currently not supported.\n";
	}
	else
	{
        cout << "Please enter a logical expression: ";
        getline(cin, choice);
        remove_spaces(choice);

        while (!is_valid_statement(choice)) {
            cout << "Please enter a logical expression: ";
            getline(cin, choice);
            remove_spaces(choice);
        }

        Statement stm = parse_string(add_parentheses(choice));

        do {
            cout << "Please select from the following:\n"
                 << "1. Apply DeMorgan's Laws\n"
                 << "2. Cancel Double Negatives\n"
                 << "0. Exit\n"
                 << ">> ";
            getline(cin, choice);
            switch(choice[0]) {
                case '0': break;
                case '1': stm.transform(DeMORGANS); break;
                case '2': stm.transform(CANCEL_NOTS); break;
                default : cout << "Invalid choice\n";
            }
            if (choice[0] != '0')
                cout << "Expression is currently: \"" << stm.to_string() << "\"\n";
        } while (choice[0] != '0');
	}
}
