#include <iostream>
#include <vector>
#include <fstream>

#include "statement.h" 
using namespace logicians;

using namespace std;

// You could use a class.
// Something along the lines of table.export(FORMAT, "filename"),
//   where FORMAT is an enum containing TXT or HTML.
void exportHTML(vector<vector<string>>);
string setup();
string closeup();

int main()
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
	
	cout << "FINISHED!\n\n";
	return 0;
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
