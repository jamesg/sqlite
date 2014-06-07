#ifndef SQLITE_TRANSACTION_HPP
#define SQLITE_TRANSACTION_HPP

/*
 * SQLite - functions for simplifying SQLite queries in C++.
 * Copyright (C) 2014 James Goode.
 */

#include <string>

#include "sqlite/connection.hpp"

namespace sqlite
{
    class transaction
    {
    public:
        /*
         * Open a new transaction with a named savepoint (so it can be rolled
         * back or committed in the destructor).
         */
        transaction(sqlite::connection&, const std::string& name);
        ~transaction();
        void commit();
        void rollback();
    private:
        sqlite::connection& m_connection;
        std::string m_savepoint_name;
        bool m_released;
    };
}

#endif

