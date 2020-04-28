#include <string>
#include <iostream>
#include <map>
#include <vector>

#include "statement.h"


namespace logicians {

    class ReplEnvironment {
        private:
            /** User created variables.
             * Can be defined like so:
             * >> (defvar $b (stm a^b))
             * >> (defvar $a $b)
             * >> (defvar $negated_a (- $a))
             *
             * Any variable name is valid as long as it
             *   begins with a '$', has no spaces, and does not contain the letter 'v'.
             * All variables are passed-by-value. There are no pointer/lazy variables.
             */
            std::map<std::string, std::string> vars;

            /** Previous input.
             * After executing the example statements
             *   in the comment for `vars`:
             * {"(defvar $b (stm a^b))", "(defvar $a $b)", "(defvar $negated_a (- $a))"}
             */
            std::vector<std::string> input;

            /** Evaluation results.
             * After executing the example statements:
             * {"a^b", "a^b", "~(a^b)"}
             */
            std::vector<std::string> results;

            std::map<std::string, std::string> flags;

            Statement::StringType stm_format = Statement::ASCII;

        public:
            ReplEnvironment(std::map<std::string, std::string> set_flags);

            /** Read in an executable statement.
             * Note: an executable statement may span more than one line.
             *   Consider "(defvar $p\n(stm p))".
             * This means we track the current depth while taking in input.
             */
            std::string read();

            /** Evaluate a statement.
             */
            void evaluate(const std::string& to_eval);

            /** Evaluate an s-expression
             */
            void evaluate_sexpr(const std::string& to_eval, int start, int end);

            /** Print the most recent evaluation result.
             */
            void print() const;

            bool loop = true;
    };
};
