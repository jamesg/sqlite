#ifndef SQLITE_UPDATE_HPP
#define SQLITE_UPDATE_HPP

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

namespace sqlite
{
    /*
     * Parameters:
     *  table  - Name of the SQL table to update
     *  fields - List of fields to update.  Indexes match the indexes in
     *           values, but the field name can be set to an empty
     *           boost::optional to omit the field.
     *  values - Boost Fusion sequence of values to update
     */
    template <typename T>
    void update(
            std::string                               table,
            std::vector<boost::optional<const char*>> fields,
            T&                                        values,
            connection&                               db
            )
    {
        std::ostringstream query;
        query << "UPDATE " << table << " SET ";
        // Fields
        json::serialise(
                fields,
                [](
                    const boost::optional<const char*>& name,
                    std::ostream& os) {
                    if( name ) os << *name << " = ?";
                },
                ", ",
                query
                );
        // ID field
        query << " WHERE " << T::id_field() << " = " << values.id();

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
            throw std::runtime_error("Preparing SQLite statement for update");
        }
        bind_values(values, stmt);

        int step_ret = sqlite3_step(stmt);
        if( step_ret != SQLITE_OK && step_ret != SQLITE_DONE )
        {
            std::ostringstream oss;
            oss << "Stepping SQLite update " << sqlite3_errmsg(db.handle());
            throw std::runtime_error(oss.str());
        }

        int finalise_ret = sqlite3_finalize(stmt);
        if( finalise_ret != SQLITE_OK && finalise_ret != SQLITE_DONE)
        {
            std::ostringstream oss;
            oss << "SQLite finalise " << sqlite3_errmsg(db.handle());
            throw std::runtime_error(oss.str());
        }
    }
}

#endif

