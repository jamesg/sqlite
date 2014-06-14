/*
 * SQLite - functions for simplifying SQLite queries in C++.
 * Copyright (C) 2014 James Goode.
 */

#include "sqlite/bind_values.hpp"

void sqlite::bind( int index, const std::string& s, sqlite3_stmt *stmt )
{
    if( sqlite3_bind_text(stmt, index, s.c_str(), s.length(), nullptr) != SQLITE_OK )
        throw std::runtime_error("Unable to bind string in SQLite query");
}
void sqlite::bind( int index, const double value, sqlite3_stmt *stmt )
{
    if( sqlite3_bind_double(stmt, index, value) != SQLITE_OK )
    {
        std::cerr << "Binding double: " << index << " " << value << std::endl;
        throw std::runtime_error("Unable to bind double in SQLite query");
    }
}
void sqlite::bind( int index, const int value, sqlite3_stmt *stmt )
{
    if( sqlite3_bind_int(stmt, index, value) != SQLITE_OK )
    {
        std::cerr << "Binding int: " << index << " " << value << std::endl;
        throw std::runtime_error("Unable to bind int in SQLite query");
    }
}

void sqlite::bind( int index, const void* value, int n, sqlite3_stmt *stmt )
{
    if( sqlite3_bind_blob(stmt, index, value, n, nullptr) != SQLITE_OK )
    {
        std::cerr << "Binding blob: " << index << std::endl;
        throw std::runtime_error("Unable to bind blod in SQLite query");
    }
}

