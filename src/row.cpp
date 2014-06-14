/*
 * SQLite - functions for simplifying SQLite queries in C++.
 * Copyright (C) 2014 James Goode.
 */

#include "sqlite/row.hpp"

sqlite::row<> sqlite::empty_row()
{
    return sqlite::row<>();
}

