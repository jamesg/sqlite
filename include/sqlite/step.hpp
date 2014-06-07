#ifndef SQLITE_STEP
#define SQLITE_STEP

/*
 * SQLite - functions for simplifying SQLite queries in C++.
 * Copyright (C) 2014 James Goode.
 */

#include <iostream>
#include <stdexcept>

#include <sqlite3.h>

namespace sqlite
{
    class connection;

    // Call sqlite3_step, converting non-success return codes into exceptions.
    void step(sqlite3_stmt *stmt);
    void step(sqlite3_stmt *stmt, connection&);
}

#endif

