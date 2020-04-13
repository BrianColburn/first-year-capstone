// TODO: Update complexities

#include <string>
#include <cctype>
#include <iostream>

#include "validate.h"

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

namespace logicians {

IF_DVC(int cnt;)

void remove_spaces(std::string& stm) {
    // For all characters in the string,
    for (std::string::iterator i = stm.begin(); i != stm.end(); ++i) {
        if (*i == ' ') // if the character is a space
            stm.erase(i); // it's gone.
    }
}

void display_err(const std::string& err, const std::string& stm, int i) {
    std::cout << "Encountered " << err << " at position " << i << std::endl;
    std::cout << stm << std::endl;
    std::cout << std::string(i, ' ');
    std::cout << '^' << std::endl;
}

int find_close_paren(const std::string& str, int i) {
    int acc = 0, // We are not nested at all.
        length = -1; // And have not traversed anything.
    if (i < 0 || i >= str.size() || str[i] != '(') {
        return length;
    } else { // `i` is valid and `str[i]` is a '('.
        acc++;
        i++;
        // While we haven't encountered the closing parentheses,
        while (acc) {
            length++; // increment the length,

            // and adjust nesting depth as needed.
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

int find_open_paren(const std::string& str, int i) {
    int acc = 0, // We are not nested at all.
        length = -1; // And have not traversed anything.
    if (i < 0 || i >= str.size() || str[i] != ')') {
        return length;
    } else { // `i` is valid and `str[i]` is a ')'.
        acc++;
        i--;
        // While we haven't found the opening parentheses,
        while (acc) {
            length++; // increment the length,

            // and adjust nesting depth as needed.
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

bool is_valid_statement(const std::string& stm) {
    IF_DVC(cnt = 0;)
    int i = 0; // The beginning is a great place to start.
    const int end = stm.size(); // O(1)
    bool is_valid = true; // Give it the benefit of the doubt.
    // All valid expressions must start with a variable, a negation, or a nested expression.
    ExprType expecting = OPERAND;
    int depth = 0; // We start at the top level,
    int length = 0; // and have not gained any length.
    char c; // The character to examine.

    if (i < end) { // We do have something to examine, right?
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

}

