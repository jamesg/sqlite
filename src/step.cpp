/*
 * SQLite - functions for simplifying SQLite queries in C++.
 * Copyright (C) 2014 James Goode.
 */

#include "sqlite/step.hpp"

#include <sstream>

#include "sqlite/connection.hpp"

void sqlite::step(sqlite3_stmt *stmt)
{
    int ret;
    while( true )
    {
        ret = sqlite3_step(stmt);
        switch( ret )
        {
            case SQLITE_DONE:
            case SQLITE_ROW:
            case SQLITE_OK:
                return;
            case SQLITE_BUSY:
                std::cerr << "Database busy; waiting..." << std::endl;
                sqlite3_sleep(250);
                break;
            case SQLITE_ERROR:
                throw std::runtime_error("Stepping SQLite query");
            default:
                throw std::runtime_error("Unhandled SQLITE return code");
        }
    }
}

void sqlite::step(sqlite3_stmt *stmt, connection& conn)
{
    int ret;
    while( true )
    {
        ret = sqlite3_step(stmt);
        switch( ret )
        {
            case SQLITE_DONE:
            case SQLITE_ROW:
            case SQLITE_OK:
                return;
            case SQLITE_BUSY:
                std::cerr << "Database busy; waiting..." << std::endl;
                sqlite3_sleep(250);
                break;
            case SQLITE_ERROR:
                {
                    std::ostringstream oss;
                    oss << "stepping sqlite query: ";
                    oss << sqlite3_errmsg(conn.handle());
                    throw std::runtime_error(oss.str());
                }
            default:
                throw std::runtime_error("Unhandled SQLITE return code");
        }
    }
}

