#ifndef validate_h
#define validate_h
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

namespace logicians {

IF_DVC(int cnt;)

/**
 * Remove spaces from a string.
 *
 * Return Value
 * -------------
 * void because the original string is mutated.
 *
 * Complexity
 * -----------
 * Time: O(stm.size())
 * Space: O(1)
 */
void remove_spaces(std::string& stm);

/** Display an error and point at a specific character.
 *
 * @param err the error message.
 * @param stm the invalid string.
 * @param i the index of the error.
 *
 * @return void since it outputs to stdout.
 *
 * \par Complexity
 * Time: O(i)
 *   I'm assuming `std::string(int,char)` is O(n)
 * Space: O(i)
 *   I'm assuming `std::string(int,char)` is O(n)
 */
void display_err(const std::string& err, const std::string& stm, int i);

/** Given a string and an index of a '(' to start from, return the length of the substring up to the matching ')'.
 * 
 * ie:
 * \code{.noparse}
 * find_close_paren("()", 0) -> 0
 * find_close_paren("(a)", 0) -> 1
 * find_close_paren("(a)", 1) -> -1
 * find_close_paren("f(x+1)", 1) -> 3
 * find_close_paren("x+1", 0) -> -1
 * \endcode
 *
 * @param str the string to search.
 * @param i the index to start from.
 * @return the length between the two parentheses.
 * 
 * \par Complexity
 * Time: O(str.size()) \n
 * Since, in the worst case scenario, we are given "(c1, c2, ..., cn)". \n
 * Space: O(1) \n
 * We only need 12 bytes for the ints because str is const&.
 */
int find_close_paren(const std::string& str, int i);

/** Given a string and an index of a ')' to start from, return the length of the substring up to the matching '('.
 * 
 * ie:
 * \code{.noparse}
 * find_open_paren("()", 1) -> 0
 * find_open_paren("(a)", 2) -> 1
 * find_open_paren("(a)", 1) -> -1
 * find_open_paren("f(x+1)", 5) -> 3
 * find_open_paren("x+1", 0) -> -1
 * \endcode
 * 
 * @param str the string to search.
 * @param i the index to start from.
 * @return the length between the two parentheses.
 * 
 * \par Complexity
 * Time: O(str.size()) \n
 *   Since, in the worst case scenario, we are given "(c1, c2, ..., cn)". \n
 * Space: O(1) \n
 *   We only need 12 bytes for the ints because str is const&. \n
 */
int find_open_paren(const std::string& str, int i);

/** \private
 */
enum ExprType {OPERAND, OPERATOR};

/** A statement is valid if all operators/operands are valid.
 *
 * @param stm the string to check
 * @return the syntax validity of the statement.
 *
 * \par Complexity
 * Time: O(stm.size()) \n
 *   Since we must traverse the entire string,
 *     however it's closer to O(number of operators) \n
 *   Great care has been taken to avoid re-checking anything and everything. \n
 * 
 * Space: O(1) \n
 *   Since we use a constant number of variables and `stm' is const&. \n
 */
bool is_valid_statement(const std::string& stm);

}
#endif

