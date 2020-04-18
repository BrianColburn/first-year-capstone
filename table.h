#include <ostream>
#include <vector>
#include <set>
#include <map>

#include "statement.h"

namespace logicians {

enum TableFormat {
    TXT,
    HTML
};

void export_table(std::ostream cout, TableFormat tfmt, Statement stm, Statement::StringType sfmt);
}
