#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>

#include "validate.h"
#include "statement.h"
#include "table.h"
#include "repl.h"

using namespace logicians;


ReplEnvironment::ReplEnvironment(std::map<std::string, std::string> set_flags) {
    flags = set_flags;
    if (flags["--debug"] == "true") {
        for (auto flag : flags)
            std::cout << flag.first << "=" << flag.second << std::endl;
    }
}


std::string ReplEnvironment::read() {
    if (std::cin.eof()) {
        loop = false;
        std::cout << std::endl;
        return "";
    }

    std::string input_line;
    std::cout << "LDSL>> ";
    getline(std::cin, input_line);

    input.push_back(input_line);
    return input.back();
}


std::string read_identifier(const std::string& expr, int i) {
    int ident_end = expr.find_first_not_of("abcdefghijklmnopqrstuwxyz", i+1);
    return expr.substr(i,ident_end - i);
}


std::string read_command(const std::string& expr, int i) {
    int ident_end = expr.find_first_of(" )", i);
    return expr.substr(i,ident_end - i);
}


std::vector<std::string> split_args(const std::string& expr, int arg_start, int arg_length) {
    std::vector<std::string> args;

    std::string tmp;
    int ix = 0;
    while (ix <= arg_length) { // -1 for the ')'
        int pos = arg_start + ix;
        if (expr[pos] == ' ' || expr[pos] == ')') {
            if (tmp == "") {
                // ignore spaces
            } else {
                // we hit the end of a lexeme
                args.push_back(tmp);
                tmp = "";
            }
            ix++;
        } else if (expr[pos] == '(') { // encountered an s-expression
            int length = find_close_paren(expr, arg_start + ix);
            args.push_back(expr.substr(pos, length + 2));
            ix += length + 2;
        } else if (expr[pos] == '"') {
            do {
                ix++;
                pos++;
                if (expr[pos] == '\\') {
                    tmp += '\\';
                    ix++;
                    pos++;
                    tmp += expr[pos];
                    ix++;
                    pos++;
                } else if (expr[pos] != '"') {
                    tmp += expr[pos];
                }
            } while (expr[pos] != '"');

            args.push_back(tmp);
            tmp = "";
            ix++;
        } else {
            tmp += expr[pos];
            ix++;
        }
    }

    if (tmp == "") {
        // We good
    } else {
        // We ended with an argument
        args.push_back(tmp);
    }

    return args;
}


bool substitute_vars(std::string& stm, std::map<std::string, std::string> vars, std::map<std::string, std::string> flags) {
    for (int sub_var_pos = stm.find("$");
             sub_var_pos != std::string::npos;
             sub_var_pos = stm.find("$")) {
        int sub_var_end = stm.find_first_not_of("abcdefghijklmnopqrstuwxyz", sub_var_pos+1);
        int sub_var_len = sub_var_end - sub_var_pos;
        std::string sub_var_name = stm.substr(sub_var_pos,sub_var_len);

        if (vars.find(sub_var_name) == vars.end()) {
            std::cout << "Undefined variable \"" << sub_var_name << "\"\n";
            return false;
        }

        std::string sub_var_value = vars[sub_var_name];
        if (flags["--debug"] == "true") {
            std::cout << "Replacing \"" << stm.substr(sub_var_pos,sub_var_len)
                      << "\" with \"" << sub_var_value
                      << "\" in expression \"" << stm << "\"\n";
        }
        stm.replace(sub_var_pos, sub_var_len,"("+sub_var_value+")");
        if (flags["--debug"] == "true") {
            std::cout << "Result: " << stm << std::endl;
        }
    }

    return true;
}


void ReplEnvironment::evaluate_sexpr(const std::string& to_eval, int start, int end) {
    std::string cmd = read_command(to_eval, start);
    int arg_start = start + cmd.size()+1;
    int arg_length = end - cmd.size() - 2;
    std::vector<std::string> args = split_args(to_eval, arg_start, arg_length);

    if (flags["--debug"] == "true") {
        std::cout << "Command: " << cmd << std::endl;
        std::cout << "Args: ";
        for (auto s : args) std::cout << s << ", ";
        std::cout << std::endl;

        if (args.size() > 0) {
            std::cout << "Processing \"" << cmd << "\" (" << cmd.size() << ") with arguments \""
                      << to_eval.substr(arg_start, arg_length) << "\" (" << arg_start << ", " << arg_length << "/" << to_eval.size() << ")\n";
        } else {
            std::cout << "Processing \"" << cmd << "\" (" << cmd.size() << ") with no arguments\n";
        }
    }

    if (cmd == "exit") {
        std::cout << "Exiting...\n";
        loop = false;
    } else if (cmd == "defvar") {
        if (args.size() == 2) {
            if (args[0][0] == '$' && args[0].find("v") == std::string::npos) {
                std::string var_name = args[0];
                int val_start = arg_start + var_name.size()+1;

                if (flags["--debug"] == "true") {
                    std::cout << "Defining \"" << var_name << "\"\n";
                    std::cout << "with value \"" << to_eval.substr(val_start, end-val_start) << "\"\n";
                }

                if (args[1][0] == '(') {
                    int length = find_close_paren(to_eval, val_start);
                    //std::cout << "length = " << length << " / " << to_eval.size() << std::endl;
                    evaluate_sexpr(args[1], 1, args[1].size()-1);

                    if (results.size() > 0) {
                        std::string result = results.back();

                        if (is_valid_statement(result)) {
                            vars[var_name] = results.back();
                            results.push_back(vars[var_name]);
                        } else {
                            std::cout << "Unable to set variable " << var_name << " to non-statement \"" << results.back() << "\"\n";
                        }
                    }
                } else if (args[1][0] == '$') {
                    // lookup var
                    std::string sub_var_name = to_eval.substr(val_start, end-val_start);
                    if (vars.find(sub_var_name) == vars.end()) {
                        std::cout << "Undefined variable \"" << sub_var_name << "\"\n";
                    } else {
                        vars[var_name] = vars[sub_var_name];
                        results.push_back(vars[var_name]);
                    }
                } else {
                    // bad val
                    std::cout << "Variable values must be another value or an s-expression\n";
                }
            } else {
                // bad var
                std::cout << "Variable names must start with a '$' and cannot contain 'v'\n";
            }
        } else {
            std::cout << "Invalid number of arguments for \"defvar\"\n"
                      << "Expected 3, recieved " << args.size() << std::endl;
        }
    } else if (cmd == "stm") {
        std::string statement = to_eval.substr(arg_start, arg_length);

        substitute_vars(statement, vars, flags);

        remove_spaces(statement);
        if (is_valid_statement(statement)) {
            results.push_back(parse_string(add_parentheses(statement)).to_string(Statement::ASCII));
        } else {
            std::cout << "\nUnable to evaluate statement \"" << statement << "\"\n";
        }
    } else if (cmd == "with-vals") {
        if (args.size() == 2) {
            std::string val_defs = args[0];
            if (val_defs.size() % 2 == 0) {
                std::map<char,bool> vals;
                for (int i = 0; i < val_defs.size(); i+=2) {
                    vals[val_defs[i]] = (val_defs[i+1] & 95) == 'T';
                }

                evaluate(args[1]);
                if (results.size() > 0) {
                    std::string result = results.back();
                    if (result[0] == 'E') {
                        std::cout << "Encountered non-statement \"" << result << "\"\n";
                    } else {
                        bool val = parse_string(results.back()).evaluate(vals);
                        if (val) {
                            std::cout << "T\n";
                            results.push_back("(~(t))v(t)");
                        } else {
                            std::cout << "F\n";
                            results.push_back("(~(f))^(f)");
                        }
                    }
                }
            }
        } else {
            std::cout << "Invalid number of arguments for \"with-vals\"\n"
                      << "Expected 2, recieved " << args.size() << std::endl;
        }
    } else if (cmd == "table") {
        if (args.size() == 3) {
            std::string filename = args[0];
            std::string sformat = args[1];
            TableFormat tformat;

            // convert the format to uppercase
            for (char& c : sformat) {
                if ('a' <= c && c <= 'z')
                    c &= 95;
            }

            if (sformat == "TXT" || sformat == "HTML") {
                if (sformat == "TXT") {
                    tformat = TXT;
                } else if (sformat == "HTML") {
                    tformat = HTML;
                }

                evaluate(args.back());
                if (results.size() > 0) {
                    std::string expr = results.back();

                    remove_spaces(expr);
                    if (is_valid_statement(expr)) {
                        Statement stm = parse_string(add_parentheses(expr));

                        if (filename == "-") {
                            export_table(std::cout, tformat, stm, stm_format);
                        } else {
                            std::ofstream file(filename, std::ofstream::out);
                            if (file.is_open()) {
                                export_table(file, tformat, stm, stm_format);
                                file.close();
                            } else {
                                std::cout << "Unable to open file \"" << filename << "\".\n";
                            }
                        }
                        results.push_back(stm.to_string());
                    }
                }
            } else {
                std::cout << "Unrecognized format \"" << sformat << "\"\n";
            }
        } else {
            std::cout << "Invalid number of arguments for \"table\"\n"
                      << "Expected 3, recieved " << args.size() << std::endl;
        }
    } else if (cmd == "transform") {
        if (args.size() > 1) {
            evaluate(args.back());
            if (results.size() > 0) {
                Statement stm = parse_string(results.back());
                for (int i = 0; i < args.size() - 1; i++) {
                    std::string tform = args[i];
                    if (tform == "demorgans") {
                        stm.transform(Statement::DeMORGANS);
                    } else if (tform == "cancelnots") {
                        stm.transform(Statement::CANCEL_NOTS);
                    } else {
                        std::cout << "Unrecognized transformation \"" << tform << "\"\n";
                    }
                }
                results.push_back(stm.to_string());
            }
        } else {
            std::cout << "Invalid number of arguments for \"transform\"\n"
                      << "Expected at least 2, recieved " << args.size() << std::endl;
        }
    } else {
        std::cout << "Invalid command \"" << cmd << "\"\n";
    }
}


void ReplEnvironment::evaluate(const std::string& to_eval) {
    if (to_eval[0] == '$' && to_eval.find("v") == std::string::npos) {
        if (vars.find(to_eval) != vars.end()) {
            results.push_back(vars[to_eval]);
        } else {
            std::cout << "Undefined variable \"" << to_eval << "\"\n";
        }
    } else if (to_eval[0] == '(') {
        int length = find_close_paren(to_eval, 0);
        //std::cout << "Matching paren found after " << length << " characters.\n";
        if (length == -1) {
            std::cout << "Incomplete s-expression: \"" << to_eval << "\"\n";
            //results.push_back("Evaluating \"" + to_eval + "\" returned no expression.");
        } else {
            evaluate_sexpr(to_eval, 1, 1+length);
        }
    } else if (to_eval[0] == '/') {
        // comments
    } else if (to_eval[0] == ':') {
        if (to_eval == ":flags") {
            for (auto flag : flags)
                std::cout << flag.first << "=" << flag.second << std::endl;
        } else {
            if (to_eval.find("=") == std::string::npos) {
                flags["--" + to_eval.substr(1)] = "true";
            } else {
                int epos = to_eval.find("=");
                flags["--" + to_eval.substr(1,epos-1)] = to_eval.substr(epos+1);
            }
        }
    }

}


void ReplEnvironment::print() const {
    if (results.size() > 0)
        std::cout << results.back() << std::endl;
}


int main(int argc, char* argv[]) {
    std::map<std::string, std::string> flags;
    std::vector<std::string> args(&argv[0], &argv[argc]);

    //std::cout << argc << std::endl;
    for (auto ptr = args.begin(); ptr != args.end(); ptr++) {
        //std::cout << *ptr << std::endl;
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

    /*for (auto f : flags)
        std::cout << "Flag: " << f.first << "=" << f.second << std::endl;
    for (auto a : args)
        std::cout << "Arg: " << a << std::endl;*/

    if (flags["--c"] == "true") {
        ReplEnvironment renv(flags);

        std::string expr;

        for (int i = 1; i < args.size(); i++)
            expr += args[i];
        renv.evaluate(expr);
        renv.print();

    } else if (args.size() == 1) {
        ReplEnvironment renv(flags);

        std::string banner = "    __               _      _                 _    ____                        _     \n"
                             "   / /  ____  ____ _(_)____(_)___ _____  ____( )  / __ \\____  ____ ___  ____ _(_)___ \n"
                             "  / /  / __ \\/ __ `/ / ___/ / __ `/ __ \\/ ___//  / / / / __ \\/ __ `__ \\/ __ `/ / __ \\\n"
                             " / /__/ /_/ / /_/ / / /__/ / /_/ / / / (__  )   / /_/ / /_/ / / / / / / /_/ / / / / /\n"
                             "/_____|____/\\__, /_/\\___/_/\\__,_/_/ /_/____/   /_____/\\____/_/ /_/ /_/\\__,_/_/_/ /_/ \n"
                             "   _____   /____/        _ _____         __                                          \n"
                             "  / ___/____  ___  _____(_) __(_)____   / /  ____ _____  ____ ___  ______ _____ ____ \n"
                             "  \\__ \\/ __ \\/ _ \\/ ___/ / /_/ / ___/  / /  / __ `/ __ \\/ __ `/ / / / __ `/ __ `/ _ \\\n"
                             " ___/ / /_/ /  __/ /__/ / __/ / /__   / /__/ /_/ / / / / /_/ / /_/ / /_/ / /_/ /  __/\n"
                             "/____/ .___/\\___/\\___/_/_/ /_/\\___/  /_____|__,_/_/ /_/\\__, /\\__,_/\\__,_/\\__, /\\___/ \n"
                             "    /_/                                               /____/            /____/       \n"
                             "Brought to you by Greenspun's Tenth Rule\n\n";
        std::cout << banner;

        while (renv.loop) {
            renv.evaluate(renv.read());
            if (renv.loop)
                renv.print();
        }
    } else if (args.size() > 3) {
        ReplEnvironment renv(flags);
        std::string filename = args[1];
        std::string format = args[2];
        std::string expr = args[3];

        // Build the statement from the remaining arguments.
        // Kinda sketchy, but it enables handling both of these:
        // ./ldsl file txt "p -> q"
        // ./ldsl file txt p -> q
        for (int i = 4; i < args.size(); i++)
            expr += args[i];

        std::string sexpr = "(table \"" + filename + "\" " + format + " (stm " + expr + "))";
        std::cout << "s-expression: " << sexpr << std::endl;

        renv.evaluate(sexpr);
    }

    return 0;
}

