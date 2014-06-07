#ifndef SQLITE_GET_COLUMN_HPP
#define SQLITE_GET_COLUMN_HPP

/*
 * SQLite - functions for simplifying SQLite queries in C++.
 * Copyright (C) 2014 James Goode.
 */

#include "sqlite/bind_values.hpp"

#include <vector>

namespace sqlite
{
    void get_column(sqlite3_stmt *stmt, const int index, double& out);
    void get_column(sqlite3_stmt *stmt, const int index, int& out);
    void get_column(sqlite3_stmt *stmt, const int index, std::string& out);
    void get_column(
            sqlite3_stmt                *stmt,
            const int                   index,
            std::vector<unsigned char>& out
            );
}

#endif

