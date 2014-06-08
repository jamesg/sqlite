#ifndef SQLITE_CONNECTION_HPP
#define SQLITE_CONNECTION_HPP

/*
 * SQLite - functions for simplifying SQLite queries in C++.
 * Copyright (C) 2014 James Goode.
 */

#include <string>

#include <boost/utility.hpp>

#include <sqlite3.h>

namespace sqlite
{
    /*
     * Resource-managing class for connections to SQLite database files.
     */
    class connection : boost::noncopyable
    {
    public:
        /*
         * Open a connection to a new in-memory database.
         */
        static connection in_memory_database();
        /*
         * Attempt to open a connection to the database file.  Throws an
         * exception if the connection could not be established.
         */
        connection(std::string filename);
        connection(connection&& o);
        ~connection();
        /*
         * Returns the internal SQLite handle representing the database
         * connection.  Required for running custom queries not assisted by
         * this class.
         */
        sqlite3 *handle();
        private:
        sqlite3 *m_handle;
    };
}

#endif

