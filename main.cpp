#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include "validate.h"
#include "statement.h"
using namespace logicians;
using namespace std;

//------------------
//html output functions
void exportHTML(vector<vector<string>>);
string setup();
string closeup();
//------------------

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
		vector<string> tempCol;
		vector<vector<string>> table;
	
		for(int i = 0; i < 25; i++)//fill table
		{
			for(int j = 0; j < 12; j++)
			{
				if(j % 2 == 0)
					tempCol.push_back("T");
				else
					tempCol.push_back("F");
			}
			table.push_back(tempCol);
			tempCol.clear();
		}	
	
		for(int i = 0; i < 25; i++)//check contents of table
		{
			for(int j = 0; j < 12; j++)
			{
				cout << table[i][j] << "   ";
			}
		
			cout << endl;
		}	
	
		exportHTML(table);
        cout << "Created a test table in html, check this file's directory.\n";
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

void exportHTML(vector<vector<string>> table)
{
	ofstream output("html-export-test.html");
	
	output << setup(); //first part of html
	
	output << "<table style=\"width:100%\">\n"; //table
	
	for(int x = 0; x < table.size(); x++)
	{
		output << "   <tr>\n";
		for(int y = 0; y < table[x].size(); y++)
		{
            // The first row will hold your Table Headers, and thus uses "TH".
            // Every row after that will hold Table Data, hence "TD".
            // ie: for "p^q", you'd have to headers {"p", "q", "p^q"}
            // and the data {{0,0,0},{1,0,0},{0,1,0},{1,1,1}}
            // You can convert statements to strings by calling "Statement::to_string()".
            if (y == 0) {
                output << "      <th>" << table[x][y] << "</th>\n";
            } else {
                output << "      <td>" << table[x][y] << "</td>\n";
            }
		}
		output << "   </tr>\n";
	}
	
	output << "   </table>";
	
	output << closeup(); //last part of html
	
	output.close();
}

string setup()
{
	return 
        "<!DOCTYPE html>"
        "<html>"
        "<head>"
        "<meta charset=\"utf-8\">"
        "<title></title>"
        "<meta name=\"author\" content=\"\">"
        "<meta name=\"description\" content=\"\">"
        "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
        "<link href=\"css/normalize.css\" rel=\"stylesheet\">"
        "<link href=\"css/style.css\" rel=\"stylesheet\">"
        "</head>"
        ""
        "<body>";
}

string closeup()
{	
	return
        "</body>"
        ""
        "</html>";
}
