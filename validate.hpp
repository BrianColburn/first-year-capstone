#include <string>
#include <cctype>
#include <iostream>

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
bool is_valid_var(const char& c) {
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
bool is_valid_not(const std::string& stm, int ix) {
    return stm[ix] == '~' && // We are at a '~'
           (is_valid_var(stm[ix+1]) || // What follows is a variable
                                       //   or a sub-expression
           is_valid_statement(stm, ix+1, find_close_paren(stm, ix+1)));
}

/**
 * A statement is valid if all operators/operands are valid.
 * 
 * Complexity
 * -----------
 * Time: Currently unknown. Possibly O(stm.size()), but I hesitate to say that
 *   because of expressions like "((~((a))))".
 *   A naive implementation could easily loop more than needed.
 * Space: I'm going to assume O(stm.size()) and hope for O(1)
 *   Because all other `is_valid_*' functions are O(1) and
 */
bool is_valid_statement(const std::string& stm, int start, int end) {
    bool is_valid = true; // Give it the benefit of the doubt.

    is_valid = parens_matched(stm); // Check parentheses

    for (int i = start; i < end; i++) {
        switch (stm[i]) {
            case '(': case ')': break; // Already checked parentheses.
            case '~': {
                if (!is_valid_not(stm, i)) {
                    is_valid = false;
                    std::cout << "Encountered invalid not at position " << i << std::endl
                              << "    " << stm.substr(i,i+3) << "...\n";
                }
                break;
            }
        }
    }

    return is_valid;
}