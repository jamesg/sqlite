#ifndef SQLITE_DETAIL_INSERT_QUERY_STRING_HPP
#define SQLITE_DETAIL_INSERT_QUERY_STRING_HPP

/*
 * SQLite - functions for simplifying SQLite queries in C++.
 * Copyright (C) 2014 James Goode.
 */

#include <string>
#include <vector>

#include <boost/optional.hpp>

namespace sqlite
{
    namespace detail
    {
        /*
         * Generate an SQL query for inserting into the given fields of a
         * table.  'Question mark' bind parameters are written to the string
         * for each field.
         */
        std::string insert_query_string(
            std::string                               table,
            std::vector<boost::optional<const char*>> fields
            );
    }
}

#endif

