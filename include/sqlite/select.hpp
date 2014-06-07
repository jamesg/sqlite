#ifndef SQLITE_SELECT_HPP
#define SQLITE_SELECT_HPP

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
     * Run a custom SELECT query, appending all matching tuples to a
     * std::vector.
     */
    template <typename T, typename Tparams>
    void select(
            connection&        db,
            const std::string& query,
            const Tparams&     params,
            std::vector<T>&    list
            )
    {
        sqlite3_stmt *stmt = nullptr;
        if( sqlite3_prepare(
                    db.handle(),
                    query.c_str(),
                    query.length(),
                    &stmt,
                    nullptr
                    ) != SQLITE_OK )
        {
            std::cerr << "Query: " << query << std::endl;
            throw std::runtime_error("Preparing SQLite statement for select");
        }

        bind_values(params, stmt);

        while( sqlite3_step(stmt) == SQLITE_ROW )
        {
            T obj;
            retrieve_values(stmt, obj);
            list.push_back(obj);
        }

        if( sqlite3_finalize(stmt) != SQLITE_OK )
            throw std::runtime_error("Finalizing SQLite statement");
    }
    /*
     * Run a custom SELECT query, appending all matching tuples to a json::list.
     */
    template <typename T, typename Tparams>
    void select(
            connection&        db,
            const std::string& query,
            const Tparams&     params,
            json::list&        list
            )
    {
        sqlite3_stmt *stmt = nullptr;
        if( sqlite3_prepare(
                    db.handle(),
                    query.c_str(),
                    query.length(),
                    &stmt,
                    nullptr
                    ) != SQLITE_OK )
        {
            std::cerr << "Query: " << query << std::endl;
            throw std::runtime_error("Preparing SQLite statement for select");
        }

        bind_values(params, stmt);

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

