/*
 * SQLite - functions for simplifying SQLite queries in C++.
 * Copyright (C) 2014 James Goode.
 */

#include "sqlite/connection.hpp"

#include <stdexcept>

sqlite::connection sqlite::connection::in_memory_database()
{
    connection c(":memory:");
    return c;
}

sqlite::connection::connection(std::string filename) :
    m_handle(nullptr)
{
    // Attempt to open a connection
    if(sqlite3_open(filename.c_str(), &m_handle) != SQLITE_OK)
    {
        if(m_handle)
        {
            sqlite3_close(m_handle);
            m_handle = nullptr;
        }
        throw std::runtime_error("Unable to open SQLite connection");
    }

    // Enable foreign keys
    sqlite3_stmt *stmt = nullptr;
    sqlite3_prepare(m_handle, "PRAGMA foreign_keys = ON", -1, &stmt, nullptr);
    int ret = sqlite3_step(stmt);
    if( ret != SQLITE_DONE && ret != SQLITE_OK )
    {
        throw std::runtime_error("enabling SQLite foreign key processing");
    }
    sqlite3_finalize(stmt);
}

sqlite::connection::connection(connection&& o) :
    m_handle(o.m_handle)
{
    o.m_handle = nullptr;
}

sqlite::connection::~connection()
{
    if(m_handle)
        sqlite3_close_v2(m_handle);
}

sqlite3 *sqlite::connection::handle()
{
    return m_handle;
}

