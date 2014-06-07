#ifndef SQLITE_BIND_VALUES_HPP
#define SQLITE_BIND_VALUES_HPP

/*
 * SQLite - functions for simplifying SQLite queries in C++.
 * Copyright (C) 2014 James Goode.
 */

#include <algorithm>
#include <iostream>
#include <stdexcept>

#include <boost/fusion/include/fold.hpp>
#include <boost/optional.hpp>

#include <sqlite3.h>

namespace sqlite
{
    void bind(int index, const std::string& s, sqlite3_stmt *stmt);
    void bind(int index, const double value, sqlite3_stmt *stmt);
    void bind(int index, const int value, sqlite3_stmt *stmt);
    void bind(int index, const void*, int l, sqlite3_stmt *stmt);

    namespace detail
    {
        /*
         * Functor used internally to bind values from a Fusion container to
         * placeholders in a SQL statement.
         */
        struct bind_fusion
        {
            bind_fusion(sqlite3_stmt *stmt) :
                m_stmt(stmt)
            {
            }

            typedef int result_type;

            template <typename T>
            int operator()(const int index, const T& val) const
            {
                bind(index, val, m_stmt);
                return index + 1;
            }

            private:
            sqlite3_stmt *m_stmt;
        };
        /*
         * Functor used internally to bind values from a Fusion container to
         * placeholders in a SQL statement.
         */
        struct bind_fusion_optional
        {
            bind_fusion_optional(
                    const std::vector<boost::optional<const char*>>::iterator
                        fields,
                   sqlite3_stmt *stmt
                   ) :
                m_fields(fields),
                m_stmt(stmt)
            {
            }

            typedef std::vector<boost::optional<const char*>>::iterator
                result_type;

            template <typename T>
            result_type operator()(const result_type& it, const T& val) const
            {
                int index = 1 +
                    std::count_if(
                            m_fields,
                            it,
                            [](boost::optional<const char*> val) {
                                return (bool)val;
                            }
                            );

                if( *it )
                    bind(index, val, m_stmt);

                result_type it2 = it;
                it2++;
                return it2;
            }
            private:
            const std::vector<boost::optional<const char*>>::iterator m_fields;
            sqlite3_stmt *m_stmt;
        };
    }

    /*
     * Bind the values in a fusion container 'container' into a statement.
     * Values in the container are substituted for ?'s in the SQL statement.
     */
    template<typename CONTAINER>
    void bind_values(CONTAINER& container, sqlite3_stmt *stmt)
    {
        // 1 because the leftmost parameter in a sqlite prepared statement has
        // index 1
        boost::fusion::fold(container, 1, detail::bind_fusion(stmt));
    }

    /*
     * Bind values in Boost Fusion container 'container' into the SQLite
     * statement.  Only bind values that are not null in 'fields', other values
     * are skipped.
     *
     * Example: The SQL contains "?, ?, ?", 'fields' contains { null, "A", "B",
     * null, "C" }, 'container' contains { "a", "b", "c", "d", "e" }.  The
     * values bound into the SQL statement will be "b", "c", "e".
     */
    template<typename CONTAINER>
    void bind_values(
            std::vector<boost::optional<const char*>> fields,
            const CONTAINER&                          container,
            sqlite3_stmt                              *stmt
            )
    {
        boost::fusion::fold(
                container,
                fields.begin(),
                detail::bind_fusion_optional(fields.begin(), stmt)
                );
    }
}

#endif

