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
                os << "<!DOCTYPE html><html><body><table>\n";

                for (int col = 0; col < table_header.size(); col++) {
                    os << "      <th>" << table_header[col] << "</th>";
                }

                os << '\n';

                for(int row = 0; row < table_data.size(); row++)
                {
                    os << "   <tr>\n";
                    for(int col = 0; col < table_header.size(); col++)
                    {
                        os << "      <td>" << table_data[row][col] << "</td>\n";
                    }
                    os << "   </tr>\n";
                }

                os << "</table></body></html>\n";

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

