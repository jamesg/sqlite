#ifndef SQLITE_GET_BY_ID_HPP
#define SQLITE_GET_BY_ID_HPP

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

#include "sqlite/bind_values.hpp"
#include "sqlite/connection.hpp"
#include "sqlite/retrieve_values.hpp"

namespace sqlite
{
    /*
     * Get a single tuple by its integer id.  The accessor type must implement
     * the static function id_field().
     */
    template <typename T>
    void get_by_id(
            const std::string&              table,
            const std::vector<std::string>& fields,
            int                             id,
            connection&                     db,
            T&                              accessor
            )
    {
        std::ostringstream query;
        query << "SELECT ";
        json::serialise(
                fields,
                (void (*)(const std::string&, std::ostream&))json::serialise,
                ", ",
                query
                );
        query << " FROM " << table << " WHERE " << T::id_field() << " = " <<
            id;

        sqlite3_stmt *stmt = nullptr;

        if( sqlite3_prepare(
                    db.handle(),
                    query.str().c_str(),
                    -1,
                    &stmt,
                    nullptr
                    ) != SQLITE_OK )
            std::cerr << "Query: " << query.str() << std::endl;

        sqlite3_bind_int(stmt, 1, id);
        if( sqlite3_step(stmt) != SQLITE_ROW )
        {
            sqlite3_finalize(stmt);
            throw std::runtime_error("Stepping SQLite get_by_id");
        }
        retrieve_values(stmt, accessor);
        if( sqlite3_finalize(stmt) != SQLITE_OK )
            throw std::runtime_error("Finalizing SQLite statement");
    }
}

#endif

