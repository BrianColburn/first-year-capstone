#pragma once
// TODO: Update complexities

#include <string>
#include <cctype>
#include <iostream>

#ifdef DEBUG_VALIDATE
  #define IF_DEBUG_VALIDATE(x) x
#else
  #define IF_DEBUG_VALIDATE(x)
#endif
#ifdef DVC // Debug Validation Complexity
  #define IF_DVC(x) x
#else
  #define IF_DVC(c)
#endif

IF_DVC(int cnt;)
IF_DVC(int depth;)

void remove_spaces(std::string& stm) {
    for (std::string::iterator i = stm.begin(); i != stm.end(); ++i) {
        if (*i == ' ')
            stm.erase(i);
    }
}

void display_err(std::string err, const std::string& stm, int ix) {
    std::cout << "Encountered " << err << " at position " << ix << std::endl;
    std::cout << stm << std::endl;
    for (int i = 0; i < ix; i++) std::cout << ' ';
    std::cout << '^' << std::endl;
}

int _is_valid_statement(const std::string& stm);
int _is_valid_statement(const std::string& stm, int start, int end);

/**
 * Given a string and an index of a '(' to start from, return the
 *   length of the substring up to the matching ')'.
 * 
 * ie:
 * find_close_paren("()", 0) -> 0
 * find_close_paren("(a)", 0) -> 1
 * find_close_paren("(a)", 1) -> -1
 * find_close_paren("f(x+1)", 1) -> 3
 * find_close_paren("x+1", 0) -> -1
 * 
 * Complexity
 * -----------
 * Time: O(str.size())
 *   Since, in the worst case scenario, we are given "(c1, c2, ..., cn)".
 * Space: O(1)
 *   We only need 12 bytes for the ints because str is const&.
 */
int find_close_paren(const std::string& str, int ix) {
    int acc = 0,
        length = -1;
    if (ix < 0 || ix >= str.size() || str[ix] != '(') {
        return length;
    } else {
        acc++;
        ix++;
        while (acc) {
            length++;

            if (str[ix] == '(') {
                acc++;
            } else if (str[ix] == ')') {
                acc--;
            }
            ix++;
        }
        return length;
    }
}

/**
 * Given a string and an index of a ')' to start from, return the
 *   length of the substring up to the matching '('.
 * 
 * ie:
 * find_open_paren("()", 1) -> 0
 * find_open_paren("(a)", 2) -> 1
 * find_open_paren("(a)", 1) -> -1
 * find_open_paren("f(x+1)", 5) -> 3
 * find_open_paren("x+1", 0) -> -1
 * 
 * Complexity
 * -----------
 * Time: O(str.size())
 *   Since, in the worst case scenario, we are given "(c1, c2, ..., cn)".
 * Space: O(1)
 *   We only need 12 bytes for the ints because str is const&.
 */
int find_open_paren(const std::string& str, int ix) {
    int acc = 0,
        length = -1;
    if (ix < 0 || ix >= str.size() || str[ix] != ')') {
        return length;
    } else {
        acc++;
        ix--;
        while (acc) {
            length++;

            if (str[ix] == ')') {
                acc++;
            } else if (str[ix] == '(') {
                acc--;
            }

            ix--;
        }
        return length;
    }
}

/**
 * Check if all parentheses are matched in an expression.
 * 
 * Complexity
 * -----------
 * Time: O(stm.size())
 *   Since the whole string must be checked.
 * Space: O(1)
 *   We only need 4 bytes for `acc' since `stm' is const&.
 */
bool parens_matched(const std::string& stm) {
    int acc = 0;
    for (int i = 0; i < stm.size(); i++) {
        if (stm[i] == '(') {
            acc++;
        } else if (stm[i] == ')') {
            acc--;
            if (acc < 0) {
                return false; // `acc' must always be positive.
                              //   otherwise, we encountered a ')' before a '('.
            }
        }
    }

    return acc == 0; // If `acc' is not 0, then there are more '('s than ')'s.
}

/**
 * A variable is valid if it matches the regex [a-uw-z].
 * 
 * Return Value
 * -----------
 * An integer representing the number of characters
 *   for the calling function to skip.
 * Positive if valid, negative if invalid.
 *
 * Parameters
 * -----------
 * const char& stm    the character to check
 *
 * Complexity
 * -----------
 * Time: O(1)
 *   Since we always check a single char.
 * Space: O(1)
 *   Since `c' is const&.
 */
int is_valid_var(const char& c) {
    bool is_valid = std::isalpha(c) && // `c' is a letter
                    std::islower(c) && // and lowercase
                    c != 'v';          // and not the `OR' operator
    return is_valid ? 1 : -1;
}

/**
 * A negation is valid if it is of the form "~var" or "~(sub-expr)"
 * 
 * Return Value
 * -----------
 * An integer representing the number of characters
 *   for the calling function to skip.
 * Positive if valid, negative if invalid.
 *
 * Parameters
 * -----------
 * const std::string& stm    the string to check
 * int ix    the position to start at
 *
 * Complexity
 * -----------
 * Time: O(stm.size())
 *   Since we might need to traverse the entire string.
 * Space: O(stm.size())
 *   While the variables allocated by this function are constant,
 *     it may call `_is_valid_statement', which will call who-knows-what.
 */
int is_valid_not(const std::string& stm, int ix) {
    IF_DVC(depth++;)
    IF_DVC(std::cout << "Call depth: " << depth << std::endl;)
    IF_DEBUG_VALIDATE(std::cout << "Entered `is_valid_not\n");
    int length = 0;
    if (stm[ix] == '~') { // We are at a '~'
        length = 1; // We'll have to skip the '~'
        if (isalpha(stm[ix+length])) { // What follows is a variable,
            int tmp = is_valid_var(stm[ix+1]); // check if the variable is valid
            length = tmp > 0 ? tmp + length : tmp - length; // skip over the variable too
        } else if (stm[ix+length] == '(') { //   a sub-expression,
            int start = ix + 1 + length; // The sub-expression starts after
                                         //   the length of the operator and parentheses.
            int end = start + find_close_paren(stm, ix+1);

            IF_DEBUG_VALIDATE(std::cout << "(ivn) Checking sub-expression\n");

            int tmp = _is_valid_statement(stm, start, end);

            IF_DEBUG_VALIDATE(std::cout << "(ivn) result of sub-expression: " << tmp << std::endl);

            length += 2; // Add space for the parentheses
            length = tmp > 0 ? tmp + length : tmp - length; // add the length of the sub-expression
            IF_DEBUG_VALIDATE(std::cout << "NOT length: " << length << std::endl);
        } else if (stm[ix+length] == '~') { //   or another negation.
            std::cout << "Warning: ";
            display_err("redundant NOT", stm, ix);
            int tmp = is_valid_not(stm, ix+1); // This could probably be written as a loop.
                                               // Left as is for now since we don't care
                                               //   *that* much about performance.
            length = tmp > 0 ? tmp + length : tmp - length;
        } else { // Everything we thought was true is false and this statement is a lie
            display_err("unexpected character", stm, ix);
            std::cout << "    Expected /[a-uw-z]/, '(', or '~'\n";
            std::cout << "    Found '" << stm[ix+length] << "'\n";
            length = -length;
        }
    }
    IF_DEBUG_VALIDATE(std::cout << "Exited `is_valid_not' with value " << length << std::endl);
    IF_DVC(depth--;)
    return length;
}

/**
 * The conjunction operation is denoted with "AND", "^", and "&".
 * The operator must have variables or sub-expressions on both sides.
 *   (ie: "p^q" or "pAND(q)")
 * 
 * Return Value
 * -----------
 * An integer representing the number of characters
 *   for the calling function to skip.
 * Positive if valid, negative if invalid.
 *
 * Parameters
 * -----------
 * const std::string& stm    the string to check
 * int ix    the position to start at
 *
 * Complexity
 * -----------
 * Time: O(stm.size())
 *   Since we may need to traverse the entire string.
 * Space: O(stm.size())
 *   While the variables allocated by this function are constant,
 *     it may call `_is_valid_statement', which will call who-knows-what.
 */
int is_valid_and(const std::string& stm, int ix) {
    IF_DVC(depth++;)
    IF_DVC(std::cout << "Call depth: " << depth << std::endl;)
    IF_DEBUG_VALIDATE(std::cout << "Entered `is_valid_and', " << stm.substr(ix) << " " << ix << "\n");
    // Make sure we're on an "and"
    bool is_valid = stm[ix] == '^'
                 || stm[ix] == '&' && stm[ix+1] != '&'
                 || stm[ix] == 'A' && stm[ix+1] == 'N' && stm[ix+2] == 'D';
    int length = stm[ix] != 'A' ? 1 : 3; // the length of the operator

    if (is_valid) {
        if (stm[ix+length] == '(') {
            int start = ix + length+1;
            int end = start + find_close_paren(stm, ix+length);
            IF_DEBUG_VALIDATE(std::cout << "(iva) Checking from " << start << " to " << end << std::endl);
            IF_DEBUG_VALIDATE(std::cout << "(iva) (\"" << stm.substr(start, end-start) << "\")\n");

            int tmp = _is_valid_statement(stm, start, end);
            IF_DEBUG_VALIDATE(std::cout << "(iva) ivs results: " << tmp << std::endl);
            length += 2; // Add space for the parentheses
            length = tmp > 0 ? tmp + length : tmp - length;
            is_valid &= length > 0;
        } else if (stm[ix+length] == '~') {
            int tmp = is_valid_not(stm, ix+length);
            length = tmp > 0 ? tmp + length : tmp - length;
            is_valid &= length > 0;
        } else if (isalpha(stm[ix+length])) {
            int tmp = is_valid_var(stm[ix+length]);
            length = tmp > 0 ? tmp + length : tmp - length;
            is_valid &= length > 0;
        } else {
            display_err("unexpected character", stm, ix);
            std::cout << "    Expected /[a-uw-z]/, '(', or '~'\n";
            std::cout << "    Found '" << stm[ix+length] << "'\n";
            length = -length;
        }
    } else {
        length = -length;
    }
    IF_DEBUG_VALIDATE(std::cout << "Exited `is_valid_and\n");
    IF_DVC(depth--;)
    return length;
}

/**
 * The disjunction operation is denoted with "OR", "v", "V", and "&&".
 * The operator must have variables or sub-expressions on both sides.
 *   (ie: "pvq" or "pOR(q)")
 * 
 * Return Value
 * -----------
 * An integer representing the number of characters
 *   for the calling function to skip.
 * Positive if valid, negative if invalid.
 *
 * Parameters
 * -----------
 * const std::string& stm    the string to check
 * int ix    the position to start at
 *
 * Complexity
 * -----------
 * Time: O(stm.size())
 *   Since we may need to traverse the entire string.
 * Space: O(stm.size())
 *   While the variables allocated by this function are constant,
 *     it may call `_is_valid_statement', which will call who-knows-what.
 */
int is_valid_or(const std::string& stm, int ix) {
    IF_DVC(depth++;)
    IF_DVC(std::cout << "Call depth: " << depth << std::endl;)
    IF_DEBUG_VALIDATE(std::cout << "Entered `is_valid_or\n");
    bool is_valid = tolower(stm[ix]) == 'v'
                 || stm[ix] == '&' && stm[ix+1] == '&'
                 || stm[ix] == 'O' && stm[ix+1] == 'R';
    int length = tolower(stm[ix]) != 'v' ? 2 : 1; // the length of the operator

    if (is_valid) {
        if (stm[ix+length] == '(') {
            int start = ix + length+1;
            int end = start + find_close_paren(stm, ix+length);
            IF_DEBUG_VALIDATE(std::cout << "(ivo) Checking from " << start << " to " << end << std::endl);
            IF_DEBUG_VALIDATE(std::cout << "(ivo) (\"" << stm.substr(start, end-start) << "\")\n");

            int tmp = _is_valid_statement(stm, start, end);
            IF_DEBUG_VALIDATE(std::cout << "(ivo) ivs results: " << tmp << std::endl);
            length += 2; // Add space for the parentheses
            length = tmp > 0 ? tmp + length : tmp - length;
            is_valid &= length > 0;
        } else if (stm[ix+length] == '~') {
            IF_DEBUG_VALIDATE(std::cout << "(ivo) Checking NOT\n");
            int tmp = is_valid_not(stm, ix+length);
            length = tmp > 0 ? tmp + length : tmp - length;
            is_valid &= length > 0;
        } else if (isalpha(stm[ix+length])) {
            int tmp = is_valid_var(stm[ix+length]);
            length = tmp > 0 ? tmp + length : tmp - length;
            is_valid &= length > 0;
        } else {
            display_err("unexpected character", stm, ix);
            std::cout << "    Expected /[a-uw-z]/, '(', or '~'\n";
            std::cout << "    Found '" << stm[ix+length] << "'\n";
            length = -length;
        }
    } else {
        length = -length;
    }
    IF_DEBUG_VALIDATE(std::cout << "OR at position " << ix << " found " << (is_valid ? "valid" : "invalid") << std::endl);
    IF_DEBUG_VALIDATE(std::cout << "Exited `is_valid_or\n");
    IF_DVC(depth--;)
    return length;
}

/**
 * Implication is denoted by "->"
 * 
 * Return Value
 * -----------
 * An integer representing the number of characters
 *   for the calling function to skip.
 * Positive if valid, negative if invalid.
 *
 * Parameters
 * -----------
 * const std::string& stm    the string to check
 * int ix    the position to start at
 *
 * Complexity
 * -----------
 * Time: O(stm.size())
 *   Since we may need to traverse the entire string.
 * Space: O(stm.size())
 *   While the variables allocated by this function are constant,
 *     it may call `_is_valid_statement', which will call who-knows-what.
 */
int is_valid_implication(const std::string& stm, int ix) {
    IF_DVC(depth++;)
    IF_DVC(std::cout << "Call depth: " << depth << std::endl;)
    IF_DEBUG_VALIDATE(std::cout << "Entered `is_valid_implication\n");
    bool is_valid = stm[ix] == '-' && stm[ix+1] == '>';
    int length = 2;

    if (is_valid) {
        if (stm[ix+length] == '(') {
            int start = ix + length+1;
            int end = start + find_close_paren(stm, ix+length);
            IF_DEBUG_VALIDATE(std::cout << "(ivi) Checking from " << start << " to " << end << std::endl);
            IF_DEBUG_VALIDATE(std::cout << "(ivi) (\"" << stm.substr(start, end-start) << "\")\n");

            int tmp = _is_valid_statement(stm, start, end);
            IF_DEBUG_VALIDATE(std::cout << "(ivi) ivs results: " << tmp << std::endl);
            length += 2; // Add space for the parentheses
            length = tmp > 0 ? tmp + length : tmp - length;
            is_valid &= length > 0;
        } else if (stm[ix+length] == '~') {
            IF_DEBUG_VALIDATE(std::cout << "(ivi) Checking NOT\n");
            int tmp = is_valid_not(stm, ix+length);
            length = tmp > 0 ? tmp + length : tmp - length;
            is_valid &= length > 0;
        } else if (isalpha(stm[ix+length])) {
            int tmp = is_valid_var(stm[ix+length]);
            length = tmp > 0 ? tmp + length : tmp - length;
            is_valid &= length > 0;
        } else {
            display_err("unexpected character", stm, ix);
            std::cout << "    Expected /[a-uw-z]/, '(', or '~'\n";
            std::cout << "    Found '" << stm[ix+length] << "'\n";
            length = -length;
        }
    } else {
        length = -length;
    }
    IF_DEBUG_VALIDATE(std::cout << "IMPLICATION at position " << ix << " found " << (is_valid ? "valid" : "invalid") << std::endl);
    IF_DEBUG_VALIDATE(std::cout << "Exited `is_valid_implication\n");
    IF_DVC(depth--;)
    return length;
}

/**
 * Equivalence is denoted by "<->" or "IFF"
 * 
 * Return Value
 * -----------
 * An integer representing the number of characters
 *   for the calling function to skip.
 * Positive if valid, negative if invalid.
 *
 * Parameters
 * -----------
 * const std::string& stm    the string to check
 * int ix    the position to start at
 *
 * Complexity
 * -----------
 * Time: O(stm.size())
 *   Since we may need to traverse the entire string.
 * Space: O(stm.size())
 *   While the variables allocated by this function are constant,
 *     it may call `_is_valid_statement', which will call who-knows-what.
 */
int is_valid_equivalence(const std::string& stm, int ix) {
    IF_DVC(depth++;)
    IF_DVC(std::cout << "Call depth: " << depth << std::endl;)
    IF_DEBUG_VALIDATE(std::cout << "Entered `is_valid_equivalence\n");
    bool is_valid = stm[ix] == '<' && stm[ix+1] == '-' && stm[ix+2] == '>'
                 || stm[ix] == 'I' && stm[ix+1] == 'F' && stm[ix+2] == 'F';
    int length = 3;

    if (is_valid) {
        if (stm[ix+length] == '(') {
            int start = ix + length+1;
            int end = start + find_close_paren(stm, ix+length);
            IF_DEBUG_VALIDATE(std::cout << "(ive) Checking from " << start << " to " << end << std::endl);
            IF_DEBUG_VALIDATE(std::cout << "(ive) (\"" << stm.substr(start, end-start) << "\")\n");

            int tmp = _is_valid_statement(stm, start, end);
            IF_DEBUG_VALIDATE(std::cout << "(ive) ivs results: " << tmp << std::endl);
            length += 2; // Add space for the parentheses
            length = tmp > 0 ? tmp + length : tmp - length;
            is_valid &= length > 0;
        } else if (stm[ix+length] == '~') {
            IF_DEBUG_VALIDATE(std::cout << "(ive) Checking NOT\n");
            int tmp = is_valid_not(stm, ix+length);
            length = tmp > 0 ? tmp + length : tmp - length;
            is_valid &= length > 0;
        } else if (isalpha(stm[ix+length])) {
            int tmp = is_valid_var(stm[ix+length]);
            length = tmp > 0 ? tmp + length : tmp - length;
            is_valid &= length > 0;
        } else {
            display_err("unexpected character", stm, ix);
            std::cout << "    Expected /[a-uw-z]/, '(', or '~'\n";
            std::cout << "    Found '" << stm[ix+length] << "'\n";
            length = -length;
        }
    } else {
        length = -length;
    }
    IF_DEBUG_VALIDATE(std::cout << "IFF at position " << ix << " found " << (is_valid ? "valid" : "invalid") << std::endl);
    IF_DEBUG_VALIDATE(std::cout << "Exited `is_valid_equivalence\n");
    IF_DVC(depth--;)
    return length;
}

/**
 * A statement is valid if all operators/operands are valid.
 * is_valid_statement traverses `stm' calling other `is_valid_*' functions
 *   as appropriate. It outputs human readable errors and warnings to stdout.
 *
 * Return Value
 * -----------
 * true if `stm' is a valid statement, false otherwise.
 *
 * Parameters
 * -----------
 * const std::string& stm    the string to check
 *
 * Complexity
 * -----------
 * Time: O(stm.size())
 *   Since we must traverse the entire string,
 *     however it's closer to O(number of operators)
 *   Great care has been taken to avoid re-checking anything and everything.
 * 
 * Space: O(stm.size())
 *   Since each sub-function uses constant space (until calling `_is_valid_statement').
 */
bool is_valid_statement(const std::string& stm) {
    return _is_valid_statement(stm) > 0;
}

enum ExprType {OPERAND, OPERATOR};

/**
 * A statement is valid if all operators/operands are valid.
 * _is_valid_statement traverses `stm' calling other `is_valid_*' functions
 *   as appropriate. It outputs human readable errors and warnings to stdout.
 * 
 * Return Value
 * -----------
 * An integer representing the number of characters
 *   for the calling function to skip.
 * Positive if valid, negative if invalid.
 *
 * Parameters
 * -----------
 * const std::string& stm    the string to check
 * const int start    the position to start at
 * const int end    the position to check up to
 *
 * Complexity
 * -----------
 * Time: O(stm.size())
 *   Since we must traverse the entire string,
 *     however it's closer to O(number of operators)
 *   Great care has been taken to avoid re-checking anything and everything.
 * 
 * Space: O(stm.size())
 *   Since each sub-function uses constant space (until calling `_is_valid_statement').
 */
int _is_valid_statement(const std::string& stm) {
    IF_DVC(cnt = 0; depth = 0;)
    int result = _is_valid_statement(stm, 0, stm.size());
    IF_DVC(std::cout << "Looped " << cnt << " times\n";)
    return result;
}

int _is_valid_statement(const std::string& stm, const int start, const int end) {
    IF_DEBUG_VALIDATE(std::cout << "Entered `_is_valid_statement\n");
    IF_DEBUG_VALIDATE(std::cout << "Testing statement \"" << stm.substr(start, end-start) << "\"\n");
    IF_DVC(depth++;)
    IF_DVC(std::cout << "Call depth: " << depth << std::endl;)
    bool is_valid = true; // Give it the benefit of the doubt.

    IF_DEBUG_VALIDATE(std::cout << start << ", " << end << ", " << end-start << "/" << stm.size() << std::endl);

    is_valid = 0 <= start
            && start < end
            && end <= stm.size();

    is_valid &= parens_matched(stm); // Check parentheses

    // Make sure stm isn't something like "(())"
    for (int i = start; is_valid && i < end; i++) {
        if (stm[i] != '(' && stm[i] != ')') {
            break; // We've hit *some kind* of other character, so we're good.
        } else if (i+1 == end) {
            is_valid &= false; // We got to the end and only saw "()"s, so it's invalid.
        }
    }

    ExprType expecting = OPERAND;

    for (int i = start; i < end; i++) {
        IF_DVC(cnt++;)
        IF_DVC(std::cout << "Loop " << cnt << ": \"" << stm.substr(i, end-i) << "\"\n";)
        switch (stm[i]) {
            case '(': {
                is_valid &= expecting == OPERAND;
                int close_paren = find_close_paren(stm, i);
                IF_DEBUG_VALIDATE(std::cout << "Encountered sub-expression \"" << stm.substr(i+1,close_paren) << "\"\n");
                int tmp = _is_valid_statement(stm, i+1, i+1+close_paren);
                i += tmp > 0 ? tmp+2-1 : -(tmp+2-1);
                if (tmp == 0) i++;
                is_valid &= tmp > 0;
                expecting = OPERATOR;
                break;
            }
            case ')': {
                is_valid &= false;
                display_err("unexpected ')'", stm, i);
                break;
            }
            case '~': {
                is_valid &= expecting == OPERAND;
                int tmp = is_valid_not(stm, i);
                IF_DEBUG_VALIDATE(std::cout << "Result of NOT: " << tmp << std::endl);
                if (tmp <= 0) {
                    is_valid &= false;
                    display_err("invalid NOT", stm, i);
                    IF_DEBUG_VALIDATE(std::cout << "Skipping " << -tmp << " \"" << stm.substr(i,i-tmp) << "\"\n");
                    IF_DEBUG_VALIDATE(std::cout << stm[i-tmp] << std::endl);
                    i -= tmp-1;
                } else {
                    IF_DEBUG_VALIDATE(std::cout << "Skipping " << tmp << " \"" << stm.substr(i,i+tmp) << "\"\n");
                    IF_DEBUG_VALIDATE(std::cout << stm[i+tmp] << std::endl);
                    i += tmp-1;
                }
                expecting = OPERATOR;
                break;
            }
            case 'A': case '^': {
                is_valid &= expecting == OPERATOR;
                int tmp = is_valid_and(stm, i);
                if (tmp <= 0) {
                    is_valid &= false;
                    display_err("invalid AND", stm, i);
                    IF_DEBUG_VALIDATE(std::cout << "Skipping " << -tmp << " \"" << stm.substr(i,i-tmp) << "\"\n");
                    IF_DEBUG_VALIDATE(std::cout << stm[i-tmp] << std::endl);
                    i -= tmp-1;
                } else {
                    IF_DEBUG_VALIDATE(std::cout << "Skipping " << tmp << " \"" << stm.substr(i,i+tmp) << "\"\n");
                    IF_DEBUG_VALIDATE(std::cout << stm[i+tmp] << std::endl);
                    i += tmp-1;
                }
                break;
            }
            case 'V': case 'v': case 'O': {
                is_valid &= expecting == OPERATOR;
                int tmp = is_valid_or(stm, i);
                if (tmp <= 0) {
                    is_valid &= false;
                    display_err("invalid OR", stm, i);
                    IF_DEBUG_VALIDATE(std::cout << "Skipping " << -tmp << " \"" << stm.substr(i,i-tmp) << "\"\n");
                    IF_DEBUG_VALIDATE(std::cout << stm[i-tmp] << std::endl);
                    i -= tmp-1;
                } else {
                    IF_DEBUG_VALIDATE(std::cout << "Skipping " << tmp << " \"" << stm.substr(i,i+tmp) << "\"\n");
                    IF_DEBUG_VALIDATE(std::cout << stm[i+tmp] << std::endl);
                    i += tmp-1;
                }
                break;
            }
            case '&': {
                is_valid &= expecting == OPERATOR;
                int tmp;
                if (stm[i+1] == '&') {
                    tmp = is_valid_or(stm,i);
                } else {
                    tmp = is_valid_and(stm,i);
                }
                if (tmp <= 0) {
                    is_valid &= false;
                    display_err("invalid &", stm, i);
                    IF_DEBUG_VALIDATE(std::cout << "Skipping " << -tmp << " \"" << stm.substr(i,i-tmp) << "\"\n");
                    IF_DEBUG_VALIDATE(std::cout << stm[i-tmp] << std::endl);
                    i -= tmp-1;
                } else {
                    IF_DEBUG_VALIDATE(std::cout << "Skipping " << tmp << " \"" << stm.substr(i,i+tmp) << "\"\n");
                    IF_DEBUG_VALIDATE(std::cout << stm[i+tmp] << std::endl);
                    i += tmp-1;
                }
                break;
            }
            case '-': {
                is_valid &= expecting == OPERATOR;
                int tmp = is_valid_implication(stm, i);
                if (tmp <= 0) {
                    is_valid &= false;
                    display_err("invalid IMPLICATION", stm, i);
                    IF_DEBUG_VALIDATE(std::cout << "Skipping " << -tmp << " \"" << stm.substr(i,i-tmp) << "\"\n");
                    IF_DEBUG_VALIDATE(std::cout << stm[i-tmp] << std::endl);
                    i -= tmp-1;
                } else {
                    IF_DEBUG_VALIDATE(std::cout << "Skipping " << tmp << " \"" << stm.substr(i,i+tmp) << "\"\n");
                    IF_DEBUG_VALIDATE(std::cout << stm[i+tmp] << std::endl);
                    i += tmp-1;
                }
                break;
            }
            case '<': case 'I': {
                is_valid &= expecting == OPERATOR;
                int tmp = is_valid_equivalence(stm, i);
                if (tmp <= 0) {
                    is_valid &= false;
                    display_err("invalid IFF", stm, i);
                    IF_DEBUG_VALIDATE(std::cout << "Skipping " << -tmp << " \"" << stm.substr(i,i-tmp) << "\"\n");
                    IF_DEBUG_VALIDATE(std::cout << stm[i-tmp] << std::endl);
                    i -= tmp-1;
                } else {
                    IF_DEBUG_VALIDATE(std::cout << "Skipping " << tmp << " \"" << stm.substr(i,i+tmp) << "\"\n");
                    IF_DEBUG_VALIDATE(std::cout << stm[i+tmp] << std::endl);
                    i += tmp-1;
                }
                break;
            }
            default: {
                int tmp = is_valid_var(stm[i]);
                if (tmp < 0) {
                    is_valid &= false;
                    display_err("unexpected character", stm, i);
                } else {
                    if (expecting == OPERATOR) {
                        is_valid &= false;
                        display_err("unexpected variable", stm, i);
                        std::cout << "    " << "Expected operator\n";
                    }
                    expecting = OPERATOR;
                    /*if (i > 1 && (is_valid_var(stm[i-1]) || stm[i-1] != '(' || stm[i-1] == ')')) {
                        is_valid &= false;
                        std::cout << "Encountered unexpected character at position " << i << std::endl
                                  << "    " << stm.substr(i,i+3) << "...\n";
                    }*/
                }
            }
        }
    }

    IF_DEBUG_VALIDATE(std::cout << "\"" << stm.substr(start, end-start) << "\" was found " << (is_valid ? "valid" : "invalid") << std::endl);
    IF_DEBUG_VALIDATE(std::cout << "Exited `_is_valid_statement\n");
    IF_DVC(depth--;)

    return is_valid ? end-start : start-end;
}
