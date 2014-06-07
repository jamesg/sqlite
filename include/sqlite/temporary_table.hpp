#ifndef SQLITE_TEMPORARY_TABLE_HPP
#define SQLITE_TEMPORARY_TABLE_HPP

/*
 * SQLite - functions for simplifying SQLite queries in C++.
 * Copyright (C) 2014 James Goode.
 */

#include "sqlite/connection.hpp"
#include "sqlite/devoid.hpp"

namespace sqlite
{
    /*
     * A 'scoped' temporary table class.  Manages a SQLite temporary table that
     * will be destroyed with this object.
     */
    class temporary_table
    {
    public:
        /*
         * Create a new temporary table.  The temporary table will be destroyed
         * with this object.  The table will be named 'name', and this name can
         * be used in other SQL queries (use the name()) function.
         * 'attribute_list' is an SQL list of attributes, for example "a INT, b
         * VARCHAR".
         */
        temporary_table(
                const std::string& name,
                const std::string& attribute_list,
                connection& conn
                ) :
            m_name(name),
            m_connection(conn)
        {
            std::ostringstream sql;
            sql << "CREATE TEMPORARY TABLE ";
            sql << name;
            sql << " (";
            sql << attribute_list;
            sql << ")";
            devoid(sql.str(), boost::fusion::vector<>(), conn);
        }

        /*
         * Drop the temporary table.
         */
        ~temporary_table()
        {
            std::ostringstream sql;
            sql << "DROP TABLE " << m_name;
            devoid(sql.str(), boost::fusion::vector<>(), m_connection);
        }

        /*
         * Get the name of the temporary table.  Allows the temporary table to
         * be used safely in SQL queries.
         */
        const std::string& name()
        {
            return m_name;
        }

    private:
        std::string m_name;
        connection& m_connection;
    };
}

#endif

