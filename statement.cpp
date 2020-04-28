#include <string>
#include <iostream>

#include <vector>
#include <set>
#include <map>
#include <algorithm>

#include "validate.h"
#include "statement.h"

//using logicians::Statement;
namespace logicians {

Statement::Statement(char c) {
    type = VAR;
    var = c;
}

Statement::Statement(Operator op, std::vector<Statement> args) {
    type = op;
    operands = args;
}

std::string Statement::to_string() const {
    return to_string(Statement::ASCII);
}

std::string Statement::to_string(const StringType& format) const {
    switch (type) {
        case Statement::VAR: return std::string(1,var);
        case Statement::NOT: {
            switch (format) {
                case Statement::ASCII: case Statement::UNICODE: return "~(" + operands[0].to_string() + ")";
                case Statement::MATH_JAX: case Statement::TEX: return "\\sim (" + operands[0].to_string() + ")";
            }
        }
        case Statement::AND: {
            switch (format) {
                case Statement::ASCII:
                    return "(" + operands[0].to_string() + ")^(" + operands[1].to_string() + ")";
                case Statement::UNICODE:
                    return "(" + operands[0].to_string() + ")∧(" + operands[1].to_string() + ")";
                case Statement::MATH_JAX: case Statement::TEX:
                    return "(" + operands[0].to_string() + ") \\land (" + operands[1].to_string() + ")";
            }

                  
        }
        case Statement::OR:  {
            switch (format) {
                case Statement::ASCII:
                    return "(" + operands[0].to_string() + ")v(" + operands[1].to_string() + ")";
                case Statement::UNICODE:
                    return "(" + operands[0].to_string() + ")∨(" + operands[1].to_string() + ")";
                case Statement::MATH_JAX: case Statement::TEX:
                    return "(" + operands[0].to_string() + ") \\lor (" + operands[1].to_string() + ")";
            }
        }
        case Statement::IFT: {
            switch (format) {
                case Statement::ASCII:
                    return "(" + operands[0].to_string() + ")->(" + operands[1].to_string() + ")";
                case Statement::UNICODE:
                    return "(" + operands[0].to_string() + ")→(" + operands[1].to_string() + ")";
                case Statement::MATH_JAX: case Statement::TEX:
                    return "(" + operands[0].to_string() + ") \\to (" + operands[1].to_string() + ")";
            }

        }
        case Statement::IFF: {
            switch (format) {
                case Statement::ASCII:
                    return "(" + operands[0].to_string() + ")<->(" + operands[1].to_string() + ")";
                case Statement::UNICODE:
                    return "(" + operands[0].to_string() + ")↔(" + operands[1].to_string() + ")";
                case Statement::MATH_JAX: case Statement::TEX:
                    return "(" + operands[0].to_string() + ") \\leftrightarrow (" + operands[1].to_string() + ")";
            }
        }
    }
    std::cout << "Statement::to_string(): Unexpected type: " << type << std::endl;
    return "_";
}

std::set<char> Statement::collect_vars() const {
    std::set<char> vset;
    switch (type) {
        case VAR: vset.insert(var); break;
        default: {
            for (Statement s : operands)
                for (char v : s.collect_vars())
                    vset.insert(v);
        }
    }
    return vset;
}

std::vector<Statement> Statement::_collect_expressions() const {
    std::vector<Statement> statements;
    for (Statement s : operands) {
        for (Statement s2 : s._collect_expressions()) {
            statements.push_back(s2);
        }
        statements.push_back(s);
    }
    return statements;
}

std::vector<Statement> Statement::collect_expressions() const {
    std::vector<Statement> statements = _collect_expressions();
    statements.push_back(*this);
    return statements;
}

std::vector<Statement> Statement::collect_expressions_ordered() const {
    std::vector<Statement> ustatements = collect_expressions();
    std::vector<Statement> statements;
    statements.push_back(ustatements[0]);
    for (int i = 1; i < ustatements.size(); i++) {
        for (std::vector<Statement>::iterator stm = statements.begin(); stm < statements.end(); stm++) {
            //std::cout << "Checking " << ustatements[i] << " and " << *stm << std::endl;
            if (*stm < ustatements[i] && stm+1 < statements.end()) {
                //std::cout << "Not inserting " << ustatements[i] << " yet\n";
            } else if (ustatements[i] == *stm) {
                //std::cout << "Already inserted " << ustatements[i] << std::endl;
                break;
            } else if (ustatements[i].type == VAR && stm->type != VAR) {
                //std::cout << "Inserting " << ustatements[i] << " before " << *stm << std::endl;
                statements.insert(stm, ustatements[i]);
                break;
            } else if (ustatements[i].type != VAR && stm->type == VAR && stm+1 < statements.end()) {
                continue;
            } else {
                //std::cout << "Inserting " << ustatements[i] << " after " << *stm << std::endl;
                statements.insert(stm+1, ustatements[i]);
                break;
            }
        }
    }
    return statements;
}

std::ostream& operator<<(std::ostream& os, const logicians::Statement& stm) {
    return os << stm.to_string();
}

bool Statement::operator==(const Statement& stm) const {
    //std::cout << *this << " =?= " << stm << " = ";
    if (type == stm.type && operands.size() == stm.operands.size()) {
        if (type == VAR) {
            //std::cout << (var == stm.var) << std::endl;
            return var == stm.var;
        } else {
            for (int i = 0; i < operands.size(); i++) {
                if (!(operands[i] == stm.operands[i])) {
                    //std::cout << 0 << std::endl;
                    return false;
                }
            }
        }
        //std::cout << 1 << std::endl;
        return true;
    } else {
        return false;
    }
}

bool Statement::operator<(const Statement& stm) const {
    //std::cout << *this << " <? " << stm << " = ";
    if (type == VAR && stm.type == VAR) {
        //std::cout << (var < stm.var) << std::endl;
        return var < stm.var;
    } else {
        for (int i = 0; i < stm.operands.size(); i++) {
            //std::cout << *this << " <? " << stm.operands[i] << " from " << stm << " = ";
            if (*this == stm.operands[i] || *this < stm.operands[i]) {
                //std::cout << 1 << std::endl;
                return true;
            }
        }
    }
    //std::cout << 0 << std::endl;
    return false;
}

void Statement::transform(const Statement::Transformation& t) {
    switch (t) {
        case Statement::DeMORGANS: {
            if (type == Statement::AND) {
                type = Statement::OR;
                operands[0].transform(t);
                operands[0] = -operands[0];
                operands[1].transform(t);
                operands[1] = -operands[1];
            } else if (type == Statement::OR) {
                type = Statement::AND;
                operands[0].transform(t);
                operands[0] = -operands[0];
                operands[1].transform(t);
                operands[1] = -operands[1];
            } else {
                for (Statement& s : operands)
                    s.transform(t);
            }
            break;
        }
        case Statement::CANCEL_NOTS: {
            if (type == Statement::NOT && operands[0].type == Statement::NOT) {
                operands[0].operands[0].transform(t);
                type = operands[0].operands[0].type;
                if (type == Statement::VAR) {
                    var = operands[0].operands[0].var;
                } else {
                    operands = operands[0].operands[0].operands;
                }
            } else {
                for (Statement& s : operands)
                    s.transform(t);
            }
            break;
        }
    }
}

Statement Statement::operator-() const {
    std::vector<Statement> v = {type == VAR ? Statement(var) : Statement(type,operands)};
    return Statement(Statement::NOT, v);
}

Statement Statement::operator*(const Statement& stm) const {
    std::vector<Statement> v = {type == VAR ? Statement(var) : Statement(type,operands),stm};
    return Statement(Statement::AND, v);
}

Statement Statement::operator+(const Statement& stm) const {
    std::vector<Statement> v = {type == VAR ? Statement(var) : Statement(type,operands),stm};
    return Statement(Statement::OR, v);
}

bool Statement::evaluate(const std::map<char,bool>& vals) const {
    switch (type) {
        case Statement::VAR: {
            try {
                return vals.at(var);
            } catch (const std::out_of_range& e) {
                std::cout << "Variable '" << var << "' does not have a value\n";
                throw e;
            }
        }
        case Statement::NOT: return !operands[0].evaluate(vals);
        case Statement::AND: return operands[0].evaluate(vals) && operands[1].evaluate(vals);
        case Statement::OR:  return operands[0].evaluate(vals) || operands[1].evaluate(vals);
        case Statement::IFT: return !operands[0].evaluate(vals) || operands[1].evaluate(vals);
        case Statement::IFF: return operands[0].evaluate(vals) == operands[1].evaluate(vals);
    }
    std::cout << "Statement::evaluate: Unexpected type: " << type << std::endl;
    return 1;
}

std::vector<std::map<char,bool>> generate_vals(const Statement& stm) {
    std::vector<std::map<char,bool>> maps;
    std::set<char> svars = stm.collect_vars();
    std::vector<char> vars(svars.begin(), svars.end());

    for (unsigned long i = 0; i < (1<<vars.size()); i++) {
        std::map<char,bool> mtmp;
        for (unsigned long j = 0; j < vars.size(); j++) {
            mtmp[vars[j]] = i & (1<<j);
        }
        maps.push_back(mtmp);
    }
    return maps;
}

Statement parse_string(std::string stm) {
    IF_DSC(parse_str++; parse_++);
    IF_DSC(std::cout << "parse_string(\"" << stm << "\")\n";)
    Statement ret = parse_vector(find_operands(stm,0));
    IF_DSC(std::cout << "Exited parse_string(\"" << stm << "\"(" << stm.size() << "))\nAfter calling `parse_string' " << parse_str << " times, `parse_vec' " << parse_vec << " times, and `parse_*' " << parse_ << " times\n");
    return ret;
}

Statement parse_vector(std::vector<std::string> vec) {
    IF_DSC(parse_vec++; parse_++);
    IF_DSC(std::cout << "parse_vector(");
    IF_DSC(for (std::string s : vec) std::cout << s << ", ");
    IF_DSC(std::cout << ")\n");
    std::vector<Statement> args;
    if (vec.size() == 1) {
        return Statement(vec[0][0]);
    } else if (!vec[0].compare("~")) {
        args.push_back(parse_vector(find_operands(vec[2],0)));
        return Statement(Statement::NOT, args);
    } else if (!vec[0].compare("AND") ||
               !vec[0].compare("&") ||
               !vec[0].compare("^")) {
        args.push_back(parse_vector(find_operands(vec[1],0)));
        args.push_back(parse_vector(find_operands(vec[2],0)));
        return Statement(Statement::AND, args);
    } else if (!vec[0].compare("OR") ||
               !vec[0].compare("V") ||
               !vec[0].compare("v") ||
               !vec[0].compare("&&")) {
        args.push_back(parse_vector(find_operands(vec[1],0)));
        args.push_back(parse_vector(find_operands(vec[2],0)));
        return Statement(Statement::OR, args);
    } else if (!vec[0].compare("->")) {
        args.push_back(parse_vector(find_operands(vec[1],0)));
        args.push_back(parse_vector(find_operands(vec[2],0)));
        return Statement(Statement::IFT, args);
    } else if (!vec[0].compare("IFF") ||
               !vec[0].compare("<->")) {
        args.push_back(parse_vector(find_operands(vec[1],0)));
        args.push_back(parse_vector(find_operands(vec[2],0)));
        return Statement(Statement::IFF, args);
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
            while (cnt > 0 && j < stm.size()) {
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

}
