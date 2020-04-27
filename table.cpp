#include <ostream>
#include <vector>
#include <set>
#include <map>

#include "statement.h"
#include "table.h"

namespace logicians
{

/** This function outputs a table to a `std::ostream`.
 */
void export_table(std::ostream& os, const TableFormat tfmt, const Statement& stm, const Statement::StringType sfmt)
{
    using namespace std;

    vector<string> table_header;
    vector<Statement> sub_expressions = stm.collect_expressions_ordered();
    for (Statement s : sub_expressions)
        table_header.push_back(s.to_string(sfmt));
    
    vector<vector<string>> table_data;
    vector<map<char, bool>> val_permutations = generate_vals(stm);
    for (map<char, bool> vals : val_permutations)
    {
        vector<string> row;
        for (Statement s : sub_expressions)
            row.push_back(s.evaluate(vals) ? "T" : "F");
        table_data.push_back(row);
    }

    // table_header = { "p", "q", "pvq"}
    // table_data   = {{"F", "F", "F"},
    //                 {"T", "F", "T"},
    //                 {"F", "T", "T"},
    //                 {"T", "T", "T"}}

    switch (tfmt) {
        case TXT:
            {
		//Xavir
		for(int i=0;i<(table_header.size());i++)
		{
			os << "| "<< table_header[i] << " "; 
		}
		os << "|\n|";
		os << string(table_header[0].size()+2, '-');
		if (table_header.size() > 1)
		{
			for(int i=1;i<(table_header.size());i++)
               		{
			os << "+";
			os << string(table_header[i].size()+2,'-');
			}
		}
		os << "|";
		for(int row=0;row<(table_data.size());row++)
		{	
			os << "\n";
			for(int col=0;col<(table_header.size());col++)
			{
				os << "| " << string(table_header[col].size()/2, ' ') << table_data[row][col] << string(table_header[col].size()/2, ' ') << " ";
			}
		os << "|";
		}
		os << "\n\n";
		break;
            }
        case HTML:
            {
                // Brandon
                os << "<!DOCTYPE html><html><head>\n"; //start of html file

                string table_css = ".logicians-table {border-collapse: collapse;}"; //html classes for the table
                string header_css = ".logicians-table-header {"
								"font-size: 25px;"
								"color: black;"
								"text-align: center;"
								"font-family: serif;"
								"border-bottom: 3px solid #1a1a1a;"
								"border-top: 3px solid #1a1a1a;}";
                 string data_css = ".logicians-table-data {"
								"font-size: 16px;"
								"color: black;"
								"text-align: center;"
								"padding: 7px;"
								"border-right: 2px solid #1a1a1a;"
								"border-left: 2px solid #1a1a1a;"
								"font-family:serif;}";
                os << "    <style>\n"
                   << string(8,' ') << table_css << endl
                   << string(8,' ') << header_css << endl
                   << string(8,' ') << data_css << endl
				   << "tr:nth-child(even){background-color: #f2f2f2}" //add a second row color
                   << "    </style>\n";

                os << "</head><body>\n<div style=\"overflow-x:auto;\">\n<table class=\"logicians-table\">\n"; //start the table

                os << "    <tr class=\"logicians-table-header-row\">\n"; //fill in table header
                for (int col = 0; col < table_header.size(); col++) {
                    os << "        <th class=\"logicians-table logicians-table-header\">" << table_header[col] << "</th>\n";
                }
                os << "    </tr>\n";

                for(int row = 0; row < table_data.size(); row++) //fill in table data
                {
                    os << "    <tr class=\"logicians-table-data-row\">\n";
                    for(int col = 0; col < table_header.size(); col++)
                    {
                        os << "      <td class=\"logicians-table-data\">" << table_data[row][col] << "</td>\n";
                    }
                    os << "    </tr>\n";
                }

                os << "</table>\n</div>\n</body></html>\n"; //end the html file

                break;
            }
        default:
            {
                os << "Invalid format\n";
                break;
            }
    }

    flush(os);
}

};

