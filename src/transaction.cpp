/*
 * SQLite - functions for simplifying SQLite queries in C++.
 * Copyright (C) 2014 James Goode.
 */

#include "sqlite/transaction.hpp"

#include <iostream>
#include <sstream>
#include <stdexcept>

#include <sqlite3.h>

sqlite::transaction::transaction(
        sqlite::connection& connection,
        const std::string&  name
        ) :
    m_connection(connection),
    m_savepoint_name(name),
    m_released(false)
{
    // Attempt to create a savepoint
    std::ostringstream os;
    os << "SAVEPOINT " << name;
    sqlite3_stmt *stmt;
    sqlite3_prepare(
            m_connection.handle(),
            os.str().c_str(), os.str().length(),
            &stmt, nullptr
            );
    if( sqlite3_step(stmt) != SQLITE_DONE )
        throw std::runtime_error("Unable to start SQLite transaction");
}

void sqlite::transaction::commit()
{
    if( m_released )
        throw std::runtime_error("Savepoint already released");
    std::ostringstream os;
    os << "RELEASE SAVEPOINT " << m_savepoint_name;
    sqlite3_stmt *stmt;
    sqlite3_prepare(
            m_connection.handle(),
            os.str().c_str(), os.str().length(),
            &stmt, nullptr
            );
    if( sqlite3_step(stmt) == SQLITE_DONE )
        m_released = true;
}

void sqlite::transaction::rollback()
{
    if( m_released )
        throw std::runtime_error("Savepoint already released");
    std::ostringstream os;
    os << "ROLLBACK TO SAVEPOINT " << m_savepoint_name;
    sqlite3_stmt *stmt;
    sqlite3_prepare(
            m_connection.handle(),
            os.str().c_str(), os.str().length(),
            &stmt, nullptr
            );
    if( sqlite3_step(stmt) == SQLITE_DONE )
        m_released = true;
}

sqlite::transaction::~transaction()
{
    // Free the savepoint
    if( !m_released )
    {
        try
        {
            rollback();
        }
        catch( const std::exception& e )
        {
            std::cerr << "Warning: failed to rollback transaction : " <<
                e.what() << std::endl;
        }
    }
}

