#ifndef SQLITE_GET_LIST
#define SQLITE_GET_LIST

/*
 * SQLite - functions for simplifying SQLite queries in C++.
 * Copyright (C) 2014 James Goode.
 */

#include <algorithm>
#include <vector>
#include <sstream>
#include <string>

#include "json/serialisers/vector.hpp"
#include "json/serialisers/string.hpp"

#include "json/json.hpp"

#include "sqlite/bind_values.hpp"
#include "sqlite/retrieve_values.hpp"

namespace sqlite
{
    /*
     * Append all tuples in a table to a json::list.
     */
    template <typename T>
    void get_list(
            connection&                     db,
            const std::string&              table,
            const std::vector<const char*>& fields,
            json::list&                     list
            )
    {
        std::ostringstream query;
        query << "SELECT DISTINCT ";
        json::serialise(
                fields,
                (void (*)(const std::string&, std::ostream&))json::serialise,
                ", ",
                query
                );
        query << " FROM " << table;

        sqlite3_stmt *stmt = nullptr;
        if( sqlite3_prepare(
                    db.handle(),
                    query.str().c_str(),
                    -1,
                    &stmt,
                    nullptr
                    ) != SQLITE_OK )
        {
            std::cerr << "Query: " << query.str() << std::endl;
            throw std::runtime_error(
                    "Preparing SQLite statement for get_list"
                    );
        }

        while( sqlite3_step(stmt) == SQLITE_ROW )
        {
            json::object obj = json::map();
            T accessor(obj);
            retrieve_values(stmt, accessor);
            list.append(obj);
        }

        if( sqlite3_finalize(stmt) != SQLITE_OK )
            throw std::runtime_error("Finalizing SQLite statement");
    }
}

#endif

