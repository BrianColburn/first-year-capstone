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
IF_DVC(int calls;)

void remove_spaces(std::string& stm) {
    for (std::string::iterator i = stm.begin(); i != stm.end(); ++i) {
        if (*i == ' ')
            stm.erase(i);
    }
}

void display_err(std::string err, const std::string& stm, int i) {
    std::cout << "Encountered " << err << " at position " << i << std::endl;
    std::cout << stm << std::endl;
    std::cout << std::string(i, ' ');
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
int find_close_paren(const std::string& str, int i) {
    int acc = 0,
        length = -1;
    if (i < 0 || i >= str.size() || str[i] != '(') {
        return length;
    } else {
        acc++;
        i++;
        while (acc) {
            length++;

            if (str[i] == '(') {
                acc++;
            } else if (str[i] == ')') {
                acc--;
            }
            i++;
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
int find_open_paren(const std::string& str, int i) {
    int acc = 0,
        length = -1;
    if (i < 0 || i >= str.size() || str[i] != ')') {
        return length;
    } else {
        acc++;
        i--;
        while (acc) {
            length++;

            if (str[i] == ')') {
                acc++;
            } else if (str[i] == '(') {
                acc--;
            }

            i--;
        }
        return length;
    }
}


enum ExprType {OPERAND, OPERATOR};
enum Expr {VAR, NOT, AND, OR, IFT, IFF};

/**
 * A statement is valid if all operators/operands are valid.
 * is_valid_statement traverses `stm' calling other `is_valid_*' functions
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
 *
 * Complexity
 * -----------
 * Time: O(stm.size())
 *   Since we must traverse the entire string,
 *     however it's closer to O(number of operators)
 *   Great care has been taken to avoid re-checking anything and everything.
 * 
 * Space: O(1)
 *   Since we use a constant number of operators and `stm' is const&.
 */
bool is_valid_statement(const std::string& stm) {
    IF_DVC(cnt = 0;)
    const int start = 0;
    const int end = stm.size(); // O(1)
    bool is_valid = true;
    ExprType expecting = OPERAND;
    int i = start;
    int depth = 0;
    int length = 0;
    char c;

    if (i < end) {
        while (i < end) {
            IF_DVC(cnt++;)
            c = stm[i];
            switch (c) {
                case '(': {
                    depth += 1;
                    if (expecting == OPERAND) {
                        // We good
                    }
                    else {
                        is_valid = false;
                        display_err("unexpected '('. Expected OPERATOR", stm, i);
                    }
                    break;
                }
                case ')': {
                    depth += -1;
                    if (depth >= 0) {
                        // We good
                    } else {
                        is_valid = false;
                        display_err("unexpected ')'", stm, i);
                    }

                    if (expecting == OPERATOR) {
                        // We good
                    }
                    else {
                        is_valid = false;
                        display_err("unexpected ')'. Expected OPERAND", stm, i);
                    }
                    break;
                }
                case '~': {
                    if (expecting == OPERAND) {
                        // We good
                    } else {
                        is_valid = false;
                        display_err("unexpected '~'. Expected OPERATOR", stm, i);
                    }
                    break;
                }
                case '&': {
                    if (stm[i+1] == '&') {
                        if (expecting == OPERATOR) {
                            // We good
                        } else {
                            is_valid = false;
                            display_err("unexpected OR. Expected OPERAND", stm, i);
                        }
                        i++;
                    } else {
                        if (expecting == OPERATOR) {
                            // We good
                        } else {
                            is_valid = false;
                            display_err("unexpected AND. Expected OPERAND", stm, i);
                        }
                    }
                    expecting = OPERAND;
                    break;
                }
                case '^': case 'A': {
                    if (c == 'A') {
                        if (stm[i+1] == 'N' && stm[i+2] == 'D') {
                            // We good
                        } else {
                            is_valid = false;
                            display_err("unexpected character(s). Expected \"AND\"", stm, i);
                        }
                        i += 2;
                    } else {
                        // We good with '^'
                    }

                    if (expecting == OPERATOR) {
                        // We good
                    } else {
                        is_valid = false;
                        display_err("unexpected AND. Expected OPERAND", stm, i);
                    }

                    expecting = OPERAND;

                    break;
                }
                case 'v': case 'V': case 'O': {
                    if (c == 'O') {
                        if (stm[i+1] == 'R') {
                            // We good
                        } else {
                            is_valid = false;
                            display_err("unexpected character. Expected 'R'", stm, i+1);
                        }
                        i += 1;
                    } else {
                        // We good with 'v' and 'V'
                    }

                    if (expecting == OPERATOR) {
                        // We good
                    } else {
                        is_valid = false;
                        display_err("unexpected OR. Expected OPERAND", stm, i);
                    }

                    expecting = OPERAND;

                    break;
                }
                case '-': {
                    if (stm[i+1] == '>') {
                            // We good
                    } else {
                        is_valid = false;
                        display_err("unexpected character. Expected '>'", stm, i+1);
                    }

                    if (expecting == OPERATOR) {
                        // We good
                    } else {
                        is_valid = false;
                        display_err("unexpected IMPLICATION. Expected OPERAND", stm, i);
                    }
                    i += 1;

                    expecting = OPERAND;

                    break;
                }
                case '<': case 'I': {
                    if (c == '<') {
                        if (stm[i+1] == '-' && stm[i+2] == '>') {
                            // We good
                        } else {
                            is_valid = false;
                            display_err("unexpected character(s). Expected \"<->\"", stm, i);
                        }
                        i += 2;
                    } else {
                        if (stm[i+1] == 'F' && stm[i+2] == 'F') {
                            // We good
                        } else {
                            is_valid = false;
                            display_err("unexpected character(s). Expected \"IFF\"", stm, i);
                        }
                        i += 2;
                    }

                    if (expecting == OPERATOR) {
                        // We good
                    } else {
                        is_valid = false;
                        display_err("unexpected IFF. Expected OPERAND", stm, i);
                    }

                    expecting = OPERAND;

                    break;
                }
                default: {
                    if (std::isalpha(c) && // `c' is a letter
                        std::islower(c) && // and lowercase
                        c != 'v') {        // and not the `OR' operator
                        if (expecting == OPERAND) {
                            // We good
                        } else {
                            is_valid = false;
                            display_err("unexpected variable. Expected OPERATOR", stm, i);
                        }
                        expecting = OPERATOR;
                    } else {
                        is_valid = false;
                        display_err("unexpected character", stm, i);
                    }
                    break;
                }
            }
            i++;
        }
    } else {
        std::cout << "Encountered unexpected nothingness at all positions";
        is_valid = false;
    }

    IF_DVC(std::cout << "Looped " << cnt << " times to check statement of length " << stm.size() << "\n";)
    return is_valid;
}

