#ifndef SQLITE_DETAIL_LAST_INSERT_ROWID_HPP
#define SQLITE_DETAIL_LAST_INSERT_ROWID_HPP

/*
 * SQLite - functions for simplifying SQLite queries in C++.
 * Copyright (C) 2014 James Goode.
 */

namespace sqlite
{
    class connection;

    namespace detail
    {
        /*
         * Get the id of the last inserted row using SQLite's native
         * last_insert_rowid() function.
         */
        int last_insert_rowid(sqlite::connection&);
    }
}

#endif

