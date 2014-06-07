/*
 * SQLite - functions for simplifying SQLite queries in C++.
 * Copyright (C) 2014 James Goode.
 */

#include "sqlite/detail/last_insert_rowid.hpp"

#include "sqlite/connection.hpp"
#include "sqlite/get_column.hpp"
#include "sqlite/step.hpp"

int sqlite::detail::last_insert_rowid(sqlite::connection& db)
{
    sqlite3_stmt *stmt = nullptr;
    sqlite3_prepare(
            db.handle(),
            "SELECT last_insert_rowid()",
            -1,
            &stmt,
            nullptr
            );
    sqlite::step(stmt);
    int rowid = 0;
    sqlite::get_column(stmt, 0, rowid);
    sqlite3_finalize(stmt);
    return rowid;
}
