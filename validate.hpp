#include <string>
#include <cctype>
#include <iostream>

#ifdef DEBUG
  #define DEBUG_IF(x) if(x)
#else
  #define DEBUG_IF(x) if(false)
#endif

int is_valid_statement(const std::string& stm, int start, int end);

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
 * Complexity
 * -----------
 * Time: O(1)
 *   Since we always check a single char.
 * Space: O(1)
 *   Since `c' is const&.
 */
int is_valid_var(const char& c) {
    return std::isalpha(c) && // `c' is a letter
           std::islower(c) && // and lowercase
           c != 'v';          // and not the `OR' operator
}

/**
 * A negation is valid if it is of the form "~var" or "~(sub-expr)"
 * 
 * Complexity
 * -----------
 * Time: O(stm.size())
 *   Since we might need to traverse the entire string.
 * Space: O(1)
 *   We only need 4 bytes for `ix' since `stm' is const&.
 */
int is_valid_not(const std::string& stm, int ix) {
    std::cout << "Entered `is_valid_not\n";
    int length = 0;
    if (stm[ix] == '~') { // We are at a '~'
        length++;
        length += is_valid_var(stm[ix+1]); // What follows is a variable
                                           //   or a sub-expression
        if (length == 1 && stm[ix+1] == '(') {
            int start = ix + 1 + length;
            int end = start + find_close_paren(stm, ix+1);
            std::cout << "(isn) Checking sub expression\n";
            int tmp = is_valid_statement(stm, start, end);
            std::cout << "(isn) result of subexpression: " << tmp << std::endl;
            length += tmp+2;
            std::cout << "NOT length: " << length << std::endl;
        }
        std::cout << "(isn) between if's with length = " << length << std::endl;
        if (length == 1) {
            std::cout << "Exited `is_valid_not' with value " << length << std::endl;
            return 0;
        }
    }
    std::cout << "Exited `is_valid_not' with value " << length << std::endl;
    return length;
}

/**
 * The conjunction operation is denoted with "AND", "^", and "&".
 * The operator must have variables or sub-expressions on both sides.
 *   (ie: "p^q" or "pAND(q)")
 * 
 * TODO: Allow "p^~q"
 * 
 * Complexity
 * -----------
 * Time: O(stm.size())
 *   Since we may need to traverse the entire string.
 * Space: O(1)
 *   Since `stm' is const&.
 */
int is_valid_and(const std::string& stm, int ix) {
    std::cout << "Entered `is_valid_and\n";
    bool is_valid = stm[ix] == '^'
                 || stm[ix] == '&' && stm[ix+1] != '&'
                 || stm[ix] == 'A' && stm[ix+1] == 'N' && stm[ix+2] == 'D';
    int length = stm[ix] != 'A' ? 1 : 3;

    if (is_valid) {
        /*
        if (stm[ix-1] == ')') {
            int tmp = is_valid_statement(stm, ix-1-find_open_paren(stm, ix-1), ix-1);
            if (tmp <= 0)
                is_valid &= false;
        } else if (!is_valid_var(stm[ix-1])) {
            is_valid &= false;
        }

        std::cout << "Finished checking left operand (" << (is_valid ? "true" : "false") << ")\n";
        */

        if (stm[ix+length] == '(') {
            int start = ix + length+1;
            int end = start + find_close_paren(stm, ix+length);
            std::cout << "(iva) Checking from " << start << " to " << end << std::endl;
            std::cout << "(iva) (\"" << stm.substr(start, end-start) << "\")\n";

            int tmp = is_valid_statement(stm, start, end);
            std::cout << "(iva) ivs results: " << tmp << std::endl;
            if (tmp > 0) length += tmp + 2;
            else is_valid &= false;
        } else if (stm[ix+length] == '~') {
            int tmp = is_valid_not(stm, ix+length);
            if (tmp > 0) length += tmp;
            else is_valid &= false;
        } else if (!is_valid_var(stm[ix+length])) {
            is_valid = false;
        }
    }
    std::cout << "Exited `is_valid_and\n";
    return is_valid ? length : 0;
}

/**
 * The disjunction operation is denoted with "OR", "v", "V", and "&&".
 * The operator must have variables or sub-expressions on both sides.
 *   (ie: "pvq" or "pOR(q)")
 * 
 * TODO: Allow "pv~q"
 * 
 * Complexity
 * -----------
 * Time: O(stm.size())
 *   Since we may need to traverse the entire string.
 * Space: O(1)
 *   Since `stm' is const&.
 */
int is_valid_or(const std::string& stm, int ix) {
    std::cout << "Entered `is_valid_or\n";
    bool is_valid = tolower(stm[ix]) == 'v'
                 || stm[ix] == '&' && stm[ix+1] == '&'
                 || stm[ix] == 'O' && stm[ix+1] == 'R';
    int length = tolower(stm[ix]) != 'v' ? 2 : 1;

    if (is_valid) {
        /*
        if (stm[ix-1] == ')') {
            is_valid_statement(stm, ix-1-find_open_paren(stm, ix-1), ix-2);
        } else if (!is_valid_var(stm[ix-1])) {
            is_valid = false;
        }
        */

        if (stm[ix+length] == '(') {
            int start = ix + length+1;
            int end = start + find_close_paren(stm, ix+length);
            std::cout << "(ivo) Checking from " << start << " to " << end << std::endl;
            std::cout << "(ivo) (\"" << stm.substr(start, end-start) << "\")\n";

            int tmp = is_valid_statement(stm, start, end);
            std::cout << "(ivo) ivs results: " << tmp << std::endl;
            if (tmp > 0) length += tmp + 2;
            else is_valid &= false;
        } else if (stm[ix+length] == '~') {
            std::cout << "(ivo) Checking NOT\n";
            int tmp = is_valid_not(stm, ix+length);
            if (tmp > 0) length += tmp;
            else is_valid &= false;
        } else if (!is_valid_var(stm[ix+length])) {
            std::cout << "(ivo) Right operand is not a variable\n";
            is_valid = false;
        }
    }
    std::cout << "OR at position " << ix << " found " << (is_valid ? "valid" : "invalid") << std::endl;
    std::cout << "Exited `is_valid_or\n";
    return is_valid ? length : 0;
}

int is_valid_implication(const std::string& stm, int ix) {
    std::cout << "Entered `is_valid_implication\n";
    bool is_valid = stm[ix] == '-' && stm[ix+1] == '>';
    int length = 2;

    if (is_valid) {
        if (stm[ix+length] == '(') {
            int start = ix + length+1;
            int end = start + find_close_paren(stm, ix+length);
            std::cout << "(ivi) Checking from " << start << " to " << end << std::endl;
            std::cout << "(ivi) (\"" << stm.substr(start, end-start) << "\")\n";

            int tmp = is_valid_statement(stm, start, end);
            std::cout << "(ivi) ivs results: " << tmp << std::endl;
            if (tmp > 0) length += tmp + 2;
            else is_valid &= false;
        } else if (stm[ix+length] == '~') {
            std::cout << "(ivi) Checking NOT\n";
            int tmp = is_valid_not(stm, ix+length);
            if (tmp > 0) length += tmp;
            else is_valid &= false;
        } else if (!is_valid_var(stm[ix+length])) {
            std::cout << "(ivi) Right operand is not a variable\n";
            is_valid = false;
        }
    }
    std::cout << "IMPLICATION at position " << ix << " found " << (is_valid ? "valid" : "invalid") << std::endl;
    std::cout << "Exited `is_valid_implication\n";
    return is_valid ? length : 0;
}

int is_valid_equivalence(const std::string& stm, int ix) {
    std::cout << "Entered `is_valid_equivalence\n";
    bool is_valid = stm[ix] == '<' && stm[ix+1] == '-' && stm[ix+2] == '>'
                 || stm[ix] == 'I' && stm[ix+1] == 'F' && stm[ix+2] == 'F';
    int length = 3;

    if (is_valid) {
        if (stm[ix+length] == '(') {
            int start = ix + length+1;
            int end = start + find_close_paren(stm, ix+length);
            std::cout << "(ive) Checking from " << start << " to " << end << std::endl;
            std::cout << "(ive) (\"" << stm.substr(start, end-start) << "\")\n";

            int tmp = is_valid_statement(stm, start, end);
            std::cout << "(ive) ivs results: " << tmp << std::endl;
            if (tmp > 0) length += tmp + 2;
            else is_valid &= false;
        } else if (stm[ix+length] == '~') {
            std::cout << "(ive) Checking NOT\n";
            int tmp = is_valid_not(stm, ix+length);
            if (tmp > 0) length += tmp;
            else is_valid &= false;
        } else if (!is_valid_var(stm[ix+length])) {
            std::cout << "(ive) Right operand is not a variable\n";
            is_valid = false;
        }
    }
    std::cout << "IFF at position " << ix << " found " << (is_valid ? "valid" : "invalid") << std::endl;
    std::cout << "Exited `is_valid_equivalence\n";
    return is_valid ? length : 0;
}

/**
 * A statement is valid if all operators/operands are valid.
 * 
 * Complexity
 * -----------
 * Time: Currently unknown. Possibly O(stm.size()), but I hesitate to say that
 *   because of expressions like "((pvq)vr)vs".
 * 
 *   This implementation does perform unneeded examinations.
 *   An example of this is when it checks the statement "((pvq)vr)vs".
 *   Stepping through the checks:
 *   1. call `parens_matched', cost: O(stm.size()) and O(1)
 *   2. Begin looping through each character
 *   3. Skip '('
 *   4. Skip '('
 *   5. Call `is_valid_var', cost: O(1) and O(1)
 *   6. Call `is_valid_or', cost: O(stm.size()) and O(1)
 *      NOTE: This is where the inefficiencies begin creeping in.
 *            `is_valid_or' re-checks p (checked at step 4)
 *   7. Call `is_valid_var'
 *   8. Skip ')'
 *   9. Call `is_valid_or'
 *      NOTE: This is where the MAJOR inefficiencies happen.
 *            This call completely re-checks "(pvq)" (checked at step 6).
 *            This only gets worse for longer expressions.
 * 
 *   Since this current implementation checks the code from left to right,
 *     we don't need to re-check anything on the left.
 * 
 * Space: I'm going to assume O(stm.size()) and hope for O(1)
 *   Because all other `is_valid_*' functions are O(1).
 */
int is_valid_statement(const std::string& stm) {
    return is_valid_statement(stm, 0, stm.size());
}

int is_valid_statement(const std::string& stm, const int start, const int end) {
    std::cout << "Entered `is_valid_statement\n";
    std::cout << "Testing statement \"" << stm.substr(start, end-start) << "\"\n";
    bool is_valid = true; // Give it the benefit of the doubt.

    std::cout << start << ", " << end << ", " << end-start << "/" << stm.size() << std::endl;

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

    for (int i = start; i < end; i++) {
        std::cout << "Checking \"" << stm.substr(i,end-i) << "\"\n";
        switch (stm[i]) {
            case '(': {
                int close_paren = find_close_paren(stm, i);
                std::cout << "Encountered sub-expression \"" << stm.substr(i+1,close_paren) << "\"\n";
                int tmp = is_valid_statement(stm, i+1, i+1+close_paren);
                if (tmp > 0)
                    i += tmp+2-1;
                else is_valid &= false;
                break;
            }
            case ')': {
                is_valid &= false;
                std::cout << "Encountered ')' at position " << i << std::endl;
                break;
            }
            case '~': {
                int tmp = is_valid_not(stm, i);
                std::cout << "Result of NOT: " << tmp << std::endl;
                if (!tmp) {
                    is_valid &= false;
                    std::cout << "Encountered invalid NOT at position " << i << std::endl
                              << "    " << stm.substr(i,i+3) << "...\n";
                } else {
                    std::cout << "Skipping " << tmp << " \"" << stm.substr(i,i+tmp) << "\"\n";
                    std::cout << stm[i+tmp] << std::endl;
                    i += tmp-1;
                }
                break;
            }
            case 'A': case '^': {
                int tmp = is_valid_and(stm, i);
                if (!tmp) {
                    is_valid &= false;
                    std::cout << "Encountered invalid AND at position " << i << std::endl
                              << "    " << stm.substr(i,i+3) << "...\n";
                } else {
                    std::cout << "Skipping " << tmp << " \"" << stm.substr(i,i+tmp) << "\"\n";
                    std::cout << stm[i+tmp] << std::endl;
                    i += tmp-1;
                }
                break;
            }
            case 'V': case 'v': case 'O': {
                int tmp = is_valid_or(stm, i);
                if (!tmp) {
                    is_valid &= false;
                    std::cout << "Encountered invalid OR at position " << i << std::endl
                              << "    " << stm.substr(i,i+3) << "...\n";
                } else {
                    std::cout << "Skipping " << tmp << " \"" << stm.substr(i,i+tmp) << "\"\n";
                    std::cout << stm[i+tmp] << std::endl;
                    i += tmp-1;
                }
                break;
            }
            case '&': {
                is_valid &= is_valid_and(stm,i) || is_valid_or(stm,i);
                break;
            }
            case '-': {
                int tmp = is_valid_implication(stm, i);
                if (!tmp) {
                    is_valid &= false;
                    std::cout << "Encountered invalid IMPLICATION at position " << i << std::endl
                              << "    " << stm.substr(i,i+3) << "...\n";
                } else {
                    std::cout << "Skipping " << tmp << " \"" << stm.substr(i,i+tmp) << "\"\n";
                    std::cout << stm[i+tmp] << std::endl;
                    i += tmp-1;
                }
                break;
            }
            case '<': case 'I': {
                int tmp = is_valid_equivalence(stm, i);
                if (!tmp) {
                    is_valid &= false;
                    std::cout << "Encountered invalid IFF at position " << i << std::endl
                              << "    " << stm.substr(i,i+3) << "...\n";
                } else {
                    std::cout << "Skipping " << tmp << " \"" << stm.substr(i,i+tmp) << "\"\n";
                    std::cout << stm[i+tmp] << std::endl;
                    i += tmp-1;
                }
                break;
            }
            default: {
                if (!is_valid_var(stm[i])) {
                    is_valid &= false;
                    std::cout << "Encountered unexpected character at position " << i << std::endl
                              << "    " << stm.substr(i,i+3) << "...\n";
                }
            }
        }
    }

    std::cout << "\"" << stm.substr(start, end-start) << "\" was found " << (is_valid ? "valid" : "invalid") << std::endl;
    std::cout << "Exited `is_valid_statement\n";

    return is_valid ? end-start : 0;
}
