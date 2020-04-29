#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include "validate.h"
#include "statement.h"
#include "table.h"
#include "repl.h"

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


/** This is a bit of a messy hack for the school servers.
 * Ideally, we could use C++17, however Riddler has GCC 4.8.
 * Thus, the two different versions of table exporting.
 * I'm not going to try supporting lower than C++11.
 */
#if (__cplusplus >= 201703L)
    #include <filesystem>

    void menu_export_table_macro() {
        Statement stm = prompt_expression();
        TableFormat table_fmt;
        string tmp_string;
        filesystem::path filename = filesystem::current_path();

        cout << "Please enter the file to output to: ";
        getline(cin, tmp_string);

        filename /= tmp_string;

        if (tmp_string == "-") {
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
                getline(cin, tmp_string);
                switch (tmp_string[0]) {
                    case '1': table_fmt = TXT; filename += ".txt"; break;
                    case '2': table_fmt = HTML; filename += ".html"; break;
                    default: cout << "Invalid choice \"" << tmp_string << "\"\n";
                }
            } while (tmp_string[0] != '1' && tmp_string[0] != '2');
        }

        if (filename.filename() == "-") {
                export_table(cout, TXT, stm, Statement::ASCII);
        } else {
            ofstream fout(filename);
            if (fout.is_open()) {
                export_table(fout, table_fmt, stm, Statement::ASCII);
                fout.close();
                cout << "The table was saved to "
                     << filename << endl
                     << "press enter to return to the menu...";
                getline(cin, tmp_string);
            }
            else
                cout << "Error opening \"" << filename << "\"\n";
        }
    }

#elif (__cplusplus >= 201103L)

    void menu_export_table_macro() {
        Statement stm = prompt_expression();
        TableFormat table_fmt;
        string tmp_string;
        string filename;

        cout << "Please enter the file to output to: ";
        getline(cin, tmp_string);

        filename = tmp_string;

        if (filename == "-") {
        } else if (filename.size() >= 4 && filename.substr(filename.size()-4) == ".txt") {
            table_fmt = TXT;
        } else if (filename.size() >= 5 && filename.substr(filename.size()-5) == ".html") {
            table_fmt = HTML;
        } else {
            do {
                cout << "Please select the output format:\n"
                     << "1. Plaintext\n"
                     << "2. HTML + CSS\n"
                     << ">> ";
                getline(cin, tmp_string);
                switch (tmp_string[0]) {
                    case '1': table_fmt = TXT; filename += ".txt"; break;
                    case '2': table_fmt = HTML; filename += ".html"; break;
                    default: cout << "Invalid choice \"" << tmp_string << "\"\n";
                }
            } while (tmp_string[0] != '1' && tmp_string[0] != '2');
        }

        if (filename == "-") {
                export_table(cout, TXT, stm, Statement::ASCII);
        } else {
            ofstream fout(filename);
            if (fout.is_open()) {
                export_table(fout, table_fmt, stm, Statement::ASCII);
                fout.close();
                cout << "The table was saved to \"" << filename << "\"\n"
                     << "press enter to return to the menu...";
                getline(cin, tmp_string);
            }
            else
                cout << "Error opening \"" << filename << "\"\n";
        }
    }

#else

/* Key parts require >= C++11 */ struct {int: -1};

#endif


void menu_transform()
{
    Statement stm = prompt_expression();
    string choice;

    do {
      cout << "Expression is currently: \"" << stm.to_string() << "\"\n";
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
    } while (choice[0] != '0');
}


void menu_help()
{
    string choice;
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
                << "press enter to return to the menu...";
            getline(cin, choice);
}


void menu_about()
{
    string choice;
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
         << "  https://github.com/BrianColburn/first-year-capstone\n"
         << "This particular binary was compiled to the C++ " << __cplusplus << " standard\n"
         << "\n"
         << "Enter 'L' to view the full license,\n"
         << "  anything else will take you to the menu.\n"
         << ">> ";
    getline(cin, choice);

    // Ta-Da! Users don't need to keep track of a LICENSE file.
    if ((choice[0] & 95) == 'L')
    {
        cout << ""
             << "MIT License"
             << "\n"
             << "Copyright (c) 2020 The Logicians: Brandon Garcia, Brian Colburn, Mark Thompson, Xavier Linares\n"
             << "\n"
             << "Permission is hereby granted, free of charge, to any person obtaining a copy\n"
             << "of this software and associated documentation files (the \"Software\"), to deal\n"
             << "in the Software without restriction, including without limitation the rights\n"
             << "to use, copy, modify, merge, publish, distribute, sublicense, and/or sell\n"
             << "copies of the Software, and to permit persons to whom the Software is\n"
             << "furnished to do so, subject to the following conditions:\n"
             << "\n"
             << "The above copyright notice and this permission notice shall be included in all\n"
             << "copies or substantial portions of the Software.\n"
             << "\n"
             << "THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\n"
             << "IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\n"
             << "FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE\n"
             << "AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\n"
             << "LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,\n"
             << "OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE\n"
             << "SOFTWARE.\n"
             << "\n"
             << "press enter to return to the menu...";
        getline(cin, choice);
    }
}

void menu_repl() {
    map<string,string> dummy_flags;
    ReplEnvironment renv(dummy_flags);

    std::string banner = "The Logicians' Domain\n"
                         "    Specific Language\n"
                         "Brought to you by Greenspun's Tenth Rule\n"
                         "Enter \"(help)\" for help\n\n";
    std::cout << banner;

    while (renv.loop) {
        renv.evaluate(renv.read());
        if (renv.loop)
            renv.print();
    }
}

struct MenuEntry {
    string text;
    void (* function)();
};


bool prompt_menu(vector<MenuEntry> menu)
{
    string choice;
    int ichoice;
    bool valid_choice = true;

    do {
        if (!valid_choice)
        {
            cout << "Invalid choice \"" << choice << "\"\n"
                 << "Please select from below:\n";
        }
        for (int i = 0; i < menu.size(); i++)
        {
            cout << (i+1) << ") " << menu[i].text << endl;
        }

        cout << (menu.size()+1) << ") Exit\n\n"
            << ">> ";
        getline(cin, choice);

        if (cin.eof())
        {
            cout << endl;
            return false;
        }

        if (choice.find_first_not_of("0123456789") == string::npos)
        {
            ichoice = stoi(choice)-1;

            valid_choice = 0 <= ichoice && ichoice <= menu.size();
        } else
        {
            valid_choice = false;
        }

    } while (!valid_choice);

    if (ichoice == menu.size())
    {
        return false;
    } else
    {
        menu[ichoice].function();
    }

    return true;
}

int main(int argc, char* argv[])
{
    map<string, string> flags;
    vector<string> args(&argv[0], &argv[argc]);

    for (auto ptr = args.begin(); ptr != args.end(); ptr++) {
      std::string arg = *ptr;
      if (arg[0] == '-' && arg[1] == '-') {
        if (arg.find("=") == std::string::npos) {
          flags[arg] = "true";
        } else {
          int epos = arg.find("=");
          flags[arg.substr(0,epos)] = arg.substr(epos+1);
        }
        args.erase(ptr);
        ptr--;
      }
    }
    if (flags["--c"] == "true") {
        ReplEnvironment renv(flags);

        std::string expr;

        for (int i = 1; i < args.size(); i++)
            expr += args[i];
        renv.evaluate(expr);
        renv.print();

    } else if (args.size() == 1) {
        vector<MenuEntry> menu;

        menu.push_back({"Create a Truth Table", menu_export_table_macro});
        menu.push_back({"Transform an Expression", menu_transform});
        menu.push_back({"REPL", menu_repl});
        menu.push_back({"Help", menu_help});
        menu.push_back({"About", menu_about});

        while (prompt_menu(menu));


    } else if (args.size() > 3) {
        ReplEnvironment renv(flags);
        std::string filename = args[1];
        std::string format = args[2];
        std::string expr = args[3];

        // Build the statement from the remaining arguments.
        // Kinda sketchy, but it enables handling both of these:
        // ./ldsl file txt "p AND q"
        // ./ldsl file txt p AND q
        for (int i = 4; i < args.size(); i++)
            expr += args[i];

        std::string sexpr = "(table \"" + filename + "\" " + format + " (stm " + expr + "))";
        if (flags["--debug"] == "true")
            std::cout << "s-expression: " << sexpr << std::endl;

        renv.evaluate(sexpr);
    }

    return 0;
}
