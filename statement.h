#ifndef statement_h
#define statement_h

#include <string>
#include <iostream>

#include <vector>
#include <set>
#include <map>
#include <algorithm>

#include "validate.h"

#ifdef DSC // Debug Statement Complexity
  #define IF_DSC(x) x
#else
  #define IF_DSC(c)
#endif

/** A namspace containing the classes and functions used in this program.
 */
namespace logicians {

IF_DSC(int calls;)

/** A class that represents boolean expressions.
 * Statement provides functionality for storing, manipulating,
 *   and displaying boolean expressions.
 */
class Statement {
    public:
        /** The operator applied by an expression.
         */
        enum Operator {
            VAR, /**< The expression is a variable. */
            NOT, /**< The expression is the negation of another expression. */
            AND, /**< The expression is the conjunction of two expressions. */
            OR,  /**< The expression is the disjunction of two expressions. */
            IFT, /**< One expression implies the other. */
            IFF  /**< Two expressions are equivalent. */
        };

        /** The output format for Statement::to_string(const StringType&).
         */
        enum StringType {
            ASCII,    /**< The string format is ASCII/plaintext. */
            UNICODE,  /**< The string format uses some unicode characters (math symbols and such). */
            MATH_JAX, /**< The string format uses mathjax syntax. */
            TEX       /**< The string format uses LaTeX syntax. */
        };

        /** The supported statement transformations
         */
        enum Transformation {
            DeMORGANS,  /**< p^q -> ~pv~q, pvq -> ~p^~q. */
            CANCEL_NOTS /**< ~~p -> p. */
        };

    private:
        Statement::Operator type; /**< The top-level operater of this expression */
        char var; /**< The character used to represent this variable */
        std::vector<Statement> operands; /**< The operands this operator is applied to */
        std::vector<Statement> _collect_expressions() const; /**< Helper method for collecting expressions */
        Statement operator-() const;
        Statement operator*(const Statement& stm) const;
        Statement operator+(const Statement& stm) const;

    public:
       /** Construct a statement containing a single variable.
         * @param c the character that represents the variable
         * \par Complexity
         *   Time: O(1) \n
         *   Space: O(1)
         */
        Statement(char c);

        /** Construct a statement by applying an operator to other expressions.
         * @param op the operator to apply.
         * @param args the expressions the operator is applied to.
         */
        Statement(Operator op, std::vector<Statement> args);

        /** Return an ASCII representation of the statement.
         * @return equivalent to Statement::to_string(ASCII).
         * @see Statement::to_string(const StringType&) const
         */
        std::string to_string() const;

        /** Return a textual representation of the statement in a given format.
         * @param format a member of the StringType enum.
         * @return a string
         *
         * \par Example
         * \code{.cpp}
         * parse_string("(a)^(b)").to_string(ASCII) -> "(a)^(b)"
         * parse_string("(a)^(b)").to_string(UNICODE) -> "(a)∧(b)"
         * parse_string("(a)^(b)").to_string(MATH_JAX) -> "(a) \land (b)"
         * parse_string("(a)^(b)").to_string(TEX) -> "(a) \land (b)"
         * \endcode
         */
        std::string to_string(const StringType& format) const;

        /** Collect all the variables of a statement.
         * @return the set containing each character for each variable.
         *
         * \par Example
         * \code{.cpp}
         * parse_string("(a)^(b)").collect_vars() -> {'a','b'}
         * \endcode
         */
        std::set<char> collect_vars() const;

        /** Collect all the expressions of a statement.
         * @return a vector containing each sub-expression and the top-level the statement.
         * De-duplication is not performed.
         *
         * \par Example
         * \code{.cpp}
         * parse_string("((a)^(b))v(~(a))").collect_expressions() ->
         *     {"a", "b", "(a)^(b)", "a", "~(a)", "((a)^(b))v(~(a))"}
         * \endcode
         */
        std::vector<Statement> collect_expressions() const;

        /** Collect all the expressions of a statement.
         * @return a vector containing each sub-expression and the top-level the statement.
         * It is guaranteed that variables come before all other expressions and each element is unique.
         *
         * \par Example
         * \code{.cpp}
         * parse_string("((a)^(b))v(~(a))").collect_expressions_ordered() ->
         *     {"a", "b", "(a)^(b)", "~(a)", "((a)^(b))v(~(a))"}
         * \endcode
         */
        std::vector<Statement> collect_expressions_ordered() const;

        /** Substitute values into a statement and evaluate it.
         * @param vals the values for each variable.
         * @return the boolean result.
         * @see generate_vals(const Statement& stm)
         *
         * \par Example
         * \code{.cpp}
         * map<char,bool> values;
         * values['p'] = true;
         * values['q'] = true;
         * parse_string("(p)^(q)").evaluate(values) -> true
         * \endcode
         */
        bool evaluate(const std::map<char,bool>& vals) const;

        /** Test if two statements are structurally identical.
         * Statements are not evaluated, so "p" != "~~p".
         * @param stm the statement to compare.
         * @return whether stm is structurally identical.
         */
        bool operator==(const Statement& stm) const;

        /** Test if two statements are structurally different.
         * Statements are not evaluated.
         * @param stm the statement to compare.
         * @return !(*this == stm)
         */
        bool operator!=(const Statement& stm) const {return !(*this == stm);}

        /** Test if an expression is contained within another.
         * i.e. "p" is a sub-expression of "p^q".
         * @param stm the possible parent statement.
         * @return whether this expression is contained within \a stm.
         */
        bool operator<(const Statement& stm) const;

        /** Apply a transformation the the expression.
         * @param t The transformation to apply.
         * @see Transformation
         */
        void transform(const Transformation& t);
};

/** Create a Statement from a string.
 * The string is assumed to be a valid statement.
 * @param stm the string to parse.
 * @return the statement represented by the string.
 */
Statement parse_string(std::string stm);

/** Assemble a Statement from a vector.
 * For internal use by `parse_string`.
 * @param vec a vector of the form {"operator", "arg1", ...}.
 * @return the statement represented by the vector.
 * @see parse_string()
 */
Statement parse_vector(std::vector<std::string> vec);

/** Separate an expression into its component parts.
 * For internal use by `parse_string`.
 * @param stm the string to separate.
 * @param ix the index to start from.
 * @return a vector of the form {"operator", "arg1", ...}.
 * @see parse_string()
 */
std::vector<std::string> find_operands(std::string stm, int ix);

/** Generate all variable cases.
 * i.e. for "a^b" the result is {{'a'=0,'b'=0}, {'a'=1,'b'=0}, {'a'=0,'b'=1}, {'a'=1,'b'=1}}.
 * @param stm the statement to pull the variables from.
 * @return a vector containing all permutations of mappings from the variable characters to boolean values.
 * @see Statement::evaluate()
 */
std::vector<std::map<char,bool>> generate_vals(const Statement& stm);

/** This functions help simplify `parse_string`.
 * By transforming expressions as follows:
 * "a&bVc->d" to "(((a)&(b))V(c))->(d)"
 * we can simply recurse into the parentheses when parsing.
 *
 * Order of operations is as follows:
 * \code{.unparsed}
 * ~
 * AND/^/&
 * OR/v/V/&&
 * ->
 * IFF/<->
 * \endcode
 *
 * @param stm the string to transform.
 * @return the transformed string.
 */
std::string add_parentheses(std::string stm);

/** Allow `cout`ing Statements.
 * Really only for debugging.
 * Use Statement::to_string() for something supported.
 */
std::ostream& operator<<(std::ostream& os, const Statement& stm);

}
#endif
