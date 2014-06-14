#ifndef SQLITE_INSERT_HPP
#define SQLITE_INSERT_HPP

/*
 * SQLite - functions for simplifying SQLite queries in C++.
 * Copyright (C) 2014 James Goode.
 */

#include <algorithm>
#include <vector>
#include <sstream>
#include <string>

#include <boost/optional.hpp>

#include "json/json.hpp"
#include "json/serialisers/vector.hpp"
#include "json/serialisers/string.hpp"

#include "sqlite/detail/insert_query_string.hpp"
#include "sqlite/detail/last_insert_rowid.hpp"
#include "sqlite/bind_values.hpp"
#include "sqlite/connection.hpp"
#include "sqlite/get_column.hpp"
#include "sqlite/step.hpp"

namespace sqlite
{
    template <typename CONTAINER>
    int insert(
            std::string                               table,
            std::vector<boost::optional<const char*>> fields,
            const CONTAINER&                          values,
            connection&                               conn
            )
    {
        std::string query_str = detail::insert_query_string(table, fields);
        sqlite3_stmt *stmt = nullptr;
        if(
            sqlite3_prepare(conn.handle(), query_str.c_str(), -1, &stmt, nullptr)
            !=
            SQLITE_OK
          )
        {
            std::ostringstream oss;
            oss << "error in SQLite insert (preparing); query: " << query_str <<
                " SQLite error: " << sqlite3_errmsg(conn.handle());
            throw std::runtime_error(oss.str());
        }
        bind_values(fields, values, stmt);
        sqlite::step(stmt, conn);
        if(sqlite3_finalize(stmt) != SQLITE_OK)
        {
            std::ostringstream oss;
            oss << "error in SQLite insert (finalising); query: " << query_str <<
                " SQLite error: " << sqlite3_errmsg(conn.handle());
            throw std::runtime_error(oss.str());
        }
        return detail::last_insert_rowid(conn);
    }

    template<typename CONTAINER>
    void insert(
            std::string table,
            std::vector<boost::optional<const char*>> fields,
            json::list& values,
            connection& conn
            )
    {
        std::string query_str = detail::insert_query_string(table, fields);
        sqlite3_stmt *stmt = nullptr;
        sqlite3_prepare(conn.handle(), query_str.c_str(), -1, &stmt, nullptr);
        for(json::object& obj : values.objects)
        {
            sqlite3_reset(stmt);
            sqlite3_clear_bindings(stmt);
            bind_values(fields, CONTAINER(obj), stmt);
            sqlite::step(stmt);
        }
    }
}

#endif

