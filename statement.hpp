#include <string>
#include <iostream>

#include <vector>
#include <set>
#include <map>

#include "validate.hpp"


enum Operator {VAR, NOT, AND, OR, IFT, IFF};

class Statement {
    private:
        Operator type;
        char var;
        std::vector<Statement> operands;
    public:
        Statement(std::string&); // Parse a statement from a string
        Statement(char); // Create a variable
        Statement(Operator, std::vector<Statement>); // Create an operator
        std::string toString() const; // For debug purposes right now
        std::set<char> collectVars() const; // Collect all the variables in the statement
        std::set<Statement> collectExpressions() const; // All the expressions
        bool evaluate(std::map<char,bool>) const; // Evaluate the statement
        friend std::ostream& operator<<(std::ostream& os, const Statement& stm);
};

Statement parse_string(std::string stm);
Statement parse_vector(std::vector<std::string> vec);
std::vector<std::string> find_operands(std::string stm, int ix);

std::ostream& operator<<(std::ostream& os, const Statement& stm) {
    switch (stm.type) {
        case VAR: os << stm.var; break;
        case NOT: os << "~(" << stm.operands[0] << ")"; break;
        case AND: os << "(" << stm.operands[0] << ")^(" << stm.operands[1] << ")"; break;
        case OR:  os << "(" << stm.operands[0] << ")v(" << stm.operands[1] << ")"; break;
        case IFT: os << "(" << stm.operands[0] << ")->(" << stm.operands[1] << ")"; break;
        case IFF: os << "(" << stm.operands[0] << ")<->(" << stm.operands[1] << ")"; break;
    }
    return os;
}

Statement::Statement(char c) {
    type = VAR;
    var = c;
}

Statement::Statement(Operator op, std::vector<Statement> args) {
    type = op;
    operands = args;
}

Statement parse_string(std::string stm) {
    //std::cout << "parse_string(\"" << stm << "\")\n";
    return parse_vector(find_operands(stm,0));
}

Statement parse_vector(std::vector<std::string> vec) {
    std::vector<Statement> args;
    if (vec.size() == 1) {
        return Statement(vec[0][0]);
    } else if (!vec[0].compare("~")) {
        args.push_back(parse_string(vec[2]));
        return Statement(NOT, args);
    } else if (!vec[0].compare("AND") ||
               !vec[0].compare("&") ||
               !vec[0].compare("^")) {
        args.push_back(parse_string(vec[1]));
        args.push_back(parse_string(vec[2]));
        return Statement(AND, args);
    } else if (!vec[0].compare("OR") ||
               !vec[0].compare("V") ||
               !vec[0].compare("v") ||
               !vec[0].compare("&&")) {
        args.push_back(parse_string(vec[1]));
        args.push_back(parse_string(vec[2]));
        return Statement(OR, args);
    } else if (!vec[0].compare("->")) {
        args.push_back(parse_string(vec[1]));
        args.push_back(parse_string(vec[2]));
        return Statement(IFT, args);
    } else if (!vec[0].compare("IFF") ||
               !vec[0].compare("<->")) {
        args.push_back(parse_string(vec[1]));
        args.push_back(parse_string(vec[2]));
        return Statement(IFF, args);
    }
    std::cout << "Encountered unexpected token \"" << vec[0] << "\"\n";
    return Statement('x');
}

std::vector<std::string> find_operands(std::string stm, int ix) {
    //std::cout << "f_o(\"" << stm << "\")\n";
    std::vector<std::string> args;

    if (stm[ix] == '(') {
        int expr_length = find_close_paren(stm,ix);
        if (expr_length == stm.size()-2) {
            return find_operands(stm.substr(ix+1,stm.size()-2), 0);
        } else {
            args.push_back(stm.substr(ix+1, expr_length));
            ix += expr_length;
            int next_arg = stm.find("(",ix);
            args.insert(args.begin(),stm.substr(ix+2,next_arg-(ix+2)));
            expr_length = find_close_paren(stm,next_arg);
            args.push_back(stm.substr(next_arg+1, expr_length));
        }
    } else if (stm[ix] == '~') {
        args.push_back("~");
        args.push_back("");
        args.push_back(stm.substr(ix+2, find_close_paren(stm, ix+1)));
    } else if (isalpha(stm[ix]) && islower(stm[ix])) {
        args.push_back(std::string(1,stm[ix]));
    } else {
        std::cout << "`find_operands': Unexpected character '" << stm[ix] << "'\n";
    }
    /*std::cout << "f_o(\"" << stm << "\") = ";
    for (std::string s : args) {
        std::cout << s << ", ";
    }
    std::cout << std::endl;*/
    
    return args;
}

/**
 * This will help simplify `parse_statement'.
 * By transforming expressions as follows:
 * "a&bVc->d" to "(((a)&(b))V(c))->(d)"
 * we can simply recurse into the parentheses when parsing.
 *
 * Order of operations is as follows:
 * ~
 * AND/^/&
 * OR/v/V/&&
 * ->
 * IFF/<->
 */
std::string add_parentheses(std::string stm) {
    int start = 0,
        end   = stm.size();
    for (int i = start; i < stm.size(); i++) {
        if (stm[i] == '<' || stm[i] == 'I') {
            stm.insert(i, ")");
            int j = i-1,
                cnt = 1;
            while (cnt > 0 && j > start) {
                if (stm[j] == ')') cnt++;
                else if (stm[j] == '(') cnt--;
                if (cnt > 0) j--;
            }
            stm.insert(j, "(");
            i+=5; // inserting the '(' and ')' shifted us 2, add another 3 to get past the "<->".
            stm.insert(i, "(");
            j = i+1;
            while (stm[j] != ')' && j < stm.size()) {
                j++;
            }
            stm.insert(j, ")");
        }
    }
    for (int i = start; i < stm.size(); i++) {
        if (stm[i] == '-' && stm[i-1] != '<') {
            stm.insert(i, ")");
            int j = i-1,
                cnt = 1;
            while (cnt > 0 && j > start) {
                if (stm[j] == ')') cnt++;
                else if (stm[j] == '(') cnt--;
                if (cnt > 0) j--;
            }
            stm.insert(j, "(");
            i+=4; // inserting the '(' and ')' shifted us 2, add another 2 to get past the "->".
            stm.insert(i, "(");
            j = i+1;
            cnt = 1;
            while (cnt > 0 && j < stm.size()-1) {
                if (stm[j] == ')') cnt--;
                else if (stm[j] == '(') cnt++;
                if (cnt > 0) j++;
            }
            stm.insert(j, ")");
        }
    }
    for (int i = start; i < stm.size(); i++) {
        if (stm[i] == 'O' || tolower(stm[i]) == 'v' || stm[i] == '&' && stm[i+1] == '&') {
            stm.insert(i, ")");
            int j = i-1,
                cnt = 1;
            while (cnt > 0 && j > start) {
                if (stm[j] == ')') cnt++;
                else if (stm[j] == '(') cnt--;
                if (cnt > 0) j--;
            }
            stm.insert(j, "(");
            if (stm[i+2] == 'O' || stm[i+2] == '&') i+=1; // get past the 'O' or '&'
            i+=3; // inserting the '(' and ')' shifted us 2, add another 1 to get past the 'R' or '&'.
            stm.insert(i, "(");
            j = i+1;
            cnt = 1;
            while (cnt > 0 && j < stm.size()) {
                if (stm[j] == ')') cnt--;
                else if (stm[j] == '(') cnt++;
                if (cnt > 0) j++;
            }
            stm.insert(j, ")");
        }
    }
    for (int i = start; i < stm.size(); i++) {
        if (stm[i] == 'A' || tolower(stm[i]) == '^' || stm[i] == '&' && stm[i+1] != '&' && stm[i-1] != '&') {
            stm.insert(i, ")");
            int j = i-1,
                cnt = 1;
            while (cnt > 0 && j > start) {
                if (stm[j] == ')') cnt++;
                else if (stm[j] == '(') cnt--;
                if (cnt > 0) j--;
            }
            stm.insert(j, "(");
            if (stm[i+2] == 'A') i+=2; // get past the "AN"
            i+=3; // inserting the '(' and ')' shifted us 2, add another 1 to get past the 'D', '^', or '&'.
            stm.insert(i, "(");
            j = i+1;
            cnt = 1;
            while (cnt > 0 && j < stm.size()) {
                if (stm[j] == ')') cnt--;
                else if (stm[j] == '(') cnt++;
                if (cnt > 0) j++;
            }
            stm.insert(j, ")");
        }
    }
    for (int i = start; i < stm.size(); i++) {
        if (stm[i] == '~') {
            i+=1; // Add 1 to get past the '~'.
            stm.insert(i, "(");
            int j = i+1,
                cnt = 1;
            while (cnt > 0 && j < stm.size()) {
                if (stm[j] == ')') cnt--;
                else if (stm[j] == '(') cnt++;
                if (cnt > 0) j++;
            }
            stm.insert(j, ")");
        }
    }
    return stm;
}
