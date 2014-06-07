#ifndef SQLITE_DEVOID_HPP
#define SQLITE_DEVOID_HPP

/*
 * SQLite - functions for simplifying SQLite queries in C++.
 * Copyright (C) 2014 James Goode.
 */

#include <algorithm>
#include <vector>
#include <sstream>
#include <string>

#include <boost/optional.hpp>

#include "json/serialisers/vector.hpp"
#include "json/serialisers/string.hpp"

#include "sqlite/bind_values.hpp"
#include "sqlite/connection.hpp"

namespace sqlite
{
    template <typename T>
    void devoid(const std::string& query, const T& values, connection& db)
    {
        sqlite3_stmt *stmt = nullptr;
        sqlite3_prepare(db.handle(), query.c_str(), -1, &stmt, nullptr);
        bind_values(values, stmt);
        int step_ret     = sqlite3_step(stmt);
        int finalise_ret = sqlite3_finalize(stmt);
        if( step_ret != SQLITE_DONE )
            throw std::runtime_error("stepping devoid SQL query");
        if( finalise_ret != SQLITE_OK )
            throw std::runtime_error("finalizing devoid SQL query");
    }
}

#endif

