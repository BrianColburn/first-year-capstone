#ifndef TABLE_H
#define TABLE_H

#include <ostream>
#include <vector>
#include <set>
#include <map>

#include "statement.h"
#include "table.h"

namespace logicians {

enum TableFormat {
    TXT,
    HTML
};

void export_table(std::ostream& cout, const TableFormat tfmt, const Statement& stm, const Statement::StringType sfmt);
}

#endif
