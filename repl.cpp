#include <string>
#include <iostream>
#include <map>
#include <vector>

#include "validate.h"
#include "statement.h"
#include "repl.h"

using namespace logicians;

void ReplInputState::move_left() {
    if (input.begin() < pos)
        pos--;
}

void ReplInputState::move_right() {
    if (pos < input.end())
        pos++;
}

void ReplInputState::insert(char c) {
    input.insert(pos,c);

    switch (c) {
        case '(':
            depth++;
            break;
        case ')':
            depth--;
            break;
    }

    if (depth == 0 && input.back() == '\n') {
        input.erase(input.end()-1);
        incomplete = false;
    }
    pos++;
}

ReplEnvironment::ReplEnvironment(std::map<std::string, std::string> set_flags) {
    flags = set_flags;
    if (flags["--debug"] == "true") {
        for (auto flag : flags)
            std::cout << flag.first << "=" << flag.second << std::endl;
    }
}

std::string ReplEnvironment::read() {
    ReplInputState ris;
    char ctmp;
    std::cout << "LDSL>> ";
    getline(std::cin, ris.input);

    /*do {
        ctmp = getch();
        if (ctmp == 27) {
            ctmp = getch();
            if (ctmp == 91) {
                ctmp = getch();
                if (ctmp == 'D') {
                    ris.move_left();
                } else if (ctmp == 'C') {
                    ris.move_right();
                }
            }
        } else {
            ris.insert(ctmp);
        }
        os << "[8G";
        os << ris.input;
        //os << "[" << (ris.input.end() - ris.pos - 1) << 'D';
        flush(os);
    } while (ris.incomplete);*/

    input.push_back(ris.input);
    return ris.input;
}

std::string read_identifier(const std::string& expr, int i) {
    int ident_end = expr.find_first_not_of("abcdefghijklmnopqrstuwxyz", i+1);
    return expr.substr(i,ident_end - i);
}

std::string read_command(const std::string& expr, int i) {
    int ident_end = expr.find(" ", i);
    return expr.substr(i,ident_end - i);
}

std::vector<std::string> split_args(const std::string& expr, int arg_start, int arg_length) {
    std::vector<std::string> args;

    std::string tmp;
    int ix = 0;
    while (ix <= arg_length) { // -1 for the ')'
        if (expr[arg_start + ix] == ' ' || expr[arg_start + ix] == ')') {
            if (tmp == "") {
                // ignore spaces
            } else {
                // we hit the end of a lexeme
                args.push_back(tmp);
                tmp = "";
            }
            ix++;
        } else if (expr[arg_start + ix] == '(') { // encountered an s-expression
            int length = find_close_paren(expr, arg_start + ix);
            args.push_back(expr.substr(arg_start + ix, length + 2));
            ix += length + 2;
        } else {
            tmp += expr[arg_start + ix];
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

bool substitute_vars(std::string& stm, std::map<std::string, std::string> vars) {
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
        /*std::cout << "Replacing \"" << var_value.substr(sub_var_pos,sub_var_len)
                  << "\" with \"" << sub_var_value
                  << "\" in expression \"" << var_value << "\"\n";*/
        stm.replace(sub_var_pos, sub_var_len,"("+sub_var_value+")");
        //std::cout << "Result: " << var_value << std::endl;
    }

    return true;
}

void ReplEnvironment::evaluate_sexpr(const std::string& to_eval, int start, int end) {
    if (to_eval == "(exit)") {
        std::cout << "Exiting...\n";
        loop = false;
        results.push_back("Evaluating \"" + to_eval + "\" returned no expression.");
        return;
    }
    std::string cmd = read_command(to_eval, start);
    int arg_start = start + cmd.size()+1;
    int arg_length = end - cmd.size() - 2;
    std::vector<std::string> args = split_args(to_eval, arg_start, arg_length);
    /*std::cout << "Processing \"" << cmd << "\" (" << cmd.size() << ") with arguments \""
              << to_eval.substr(arg_start, arg_length) << "\" (" << arg_start << ", " << arg_length << "/" << to_eval.size() << ")\n";*/

    /*std::cout << "Command: " << cmd << std::endl;
    std::cout << "Args: ";
    for (auto s : args) std::cout << s << ", ";
    std::cout << std::endl;*/

    if (cmd == "defvar") {
        if (args.size() == 2) {
            if (args[0][0] == '$' && args[0].find("v") == std::string::npos) {
                std::string var_name = args[0];
                int val_start = arg_start + var_name.size()+1;

                //std::cout << "Defining \"" << var_name << "\"\n";
                //std::cout << "with value \"" << to_eval.substr(val_start, end-val_start) << "\"\n";

                if (args[1][0] == '(') {
                    int length = find_close_paren(to_eval, val_start);
                    //std::cout << "length = " << length << " / " << to_eval.size() << std::endl;
                    evaluate_sexpr(args[1], 1, args[1].size()-1);
                    if (results.back()[0] != 'E') {
                        vars[var_name] = results.back();
                        results.push_back(vars[var_name]);
                        return;
                    } else {
                        std::cout << "Unable to set variable " << var_name << " to non-statement \"" << results.back() << "\"\n";
                    }
                } else if (args[1][0] == '$') {
                    // lookup var
                    std::string sub_var_name = to_eval.substr(val_start, end-val_start);
                    if (vars.find(sub_var_name) == vars.end()) {
                        std::cout << "Undefined variable \"" << sub_var_name << "\"\n";
                    } else {
                        vars[var_name] = vars[sub_var_name];
                        results.push_back(vars[var_name]);
                        return;
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
            std::cout << "Invalid number of arguments: " << args.size() << std::endl
                      << "Expected 2\n";
        }
    } else if (cmd == "stm") {
        std::string statement = to_eval.substr(arg_start, arg_length);

        substitute_vars(statement, vars);

        remove_spaces(statement);
        if (is_valid_statement(statement)) {
            results.push_back(parse_string(add_parentheses(statement)).to_string(ASCII));
            return;
        } else {
            std::cout << "Unable to evaluate statement \"" << statement << "\"\n";
        }
    } else if (cmd == "with-vals") {
        std::map<char,bool> vals;
        int ix = arg_start;
        while (to_eval[ix] != ' ') {
            vals[to_eval[ix]] = to_eval[ix+1] == 'T';
            ix += 2;
        }

        ix++;

        if (to_eval[ix] == '(') {
            int length = find_close_paren(to_eval, ix);
            //std::cout << "length = " << length << " / " << to_eval.size() << std::endl;
            evaluate_sexpr(to_eval, ix+1, length+1);
        } else if (to_eval[ix] == '$') {
            // lookup var
            std::string sub_var_name = to_eval.substr(ix, end-ix);
            if (vars.find(sub_var_name) == vars.end()) {
                std::cout << "Undefined variable \"" << sub_var_name << "\"\n";
                results.push_back("Evaluating \"" + to_eval + "\" returned no expression.");
                return;
            } else {
                results.push_back(vars[sub_var_name]);
            }
        }

        results.push_back(parse_string(results.back()).evaluate(vals) ? "T" : "F");
        return;
    } else if (cmd == "table") {
        std::string filename = args[0];
        std::string format = args[1];

        // convert the format to uppercase
        for (char& c : format) {
            if ('a' <= c && c <= 'z')
                c &= 95;
        }

        if (format == "TXT" || format == "HTML") {
            std::cout << "Outputting a " << format << " table to \"" << filename << "\"\n";

            evaluate(args.back());
            std::string expr = results.back();

            std::cout << "Input: \"" << expr << "\"\n";
            remove_spaces(expr);
            if (is_valid_statement(expr)) {
                Statement stm = parse_string(add_parentheses(expr));
                std::cout << "stm = parse_string(\"" << stm.to_string() << "\")\n";
                std::cout << "export_table(\"" << filename << "\", " << format << ", parse_string(\"" << stm.to_string() << "\")\n";
                results.push_back(stm.to_string());
                return;
            }
        } else {
            std::cout << "Unrecognized format \"" << format << "\"\n";
        }
   }

    results.push_back("Evaluating \"" + to_eval + "\" returned no expression.");
}

void ReplEnvironment::evaluate(const std::string& to_eval) {
    if (to_eval[0] == '$' && to_eval.find("v") == std::string::npos) {
        if (vars.find(to_eval) != vars.end()) {
            results.push_back(vars[to_eval]);
            return;
        } else {
            std::cout << "Undefined variable \"" << to_eval << "\"\n";
            return;
        }
    } else if (to_eval[0] == '(') {
        int length = find_close_paren(to_eval, 0);
        //std::cout << "Matching paren found after " << length << " characters.\n";
        if (length == -1) {
            std::cout << "Incomplete s-expression: \"" << to_eval << "\"\n";
            results.push_back("Evaluating \"" + to_eval + "\" returned no expression.");
        } else {
            evaluate_sexpr(to_eval, 1, 1+length);
        }
        return;
    }

    std::vector<std::string> lexemes;
    for (int i = 0, lastspace = 0; i < to_eval.size(); i++) {
        if (to_eval[i] == ' ') {
            lexemes.push_back(to_eval.substr(lastspace, i-lastspace));
            lastspace = i+1;
        } else if (i+1 == to_eval.size()) {
            lexemes.push_back(to_eval.substr(lastspace));
        }
    }

    for (int i = 0; i < lexemes.size(); i++) {
        std::string lex = lexemes[i];
        //std::cout << lex << std::endl;
    }
    results.push_back("Evaluating \"" + to_eval + "\" returned no expression.");
}

void ReplEnvironment::print() const {
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

        std::string sexpr = "(table " + filename + " " + format + " (stm " + expr + "))";
        std::cout << "s-expression: " << sexpr << std::endl;

        renv.evaluate(sexpr);
    }

    return 0;
}

