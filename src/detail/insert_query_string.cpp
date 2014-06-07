/*
 * SQLite - functions for simplifying SQLite queries in C++.
 * Copyright (C) 2014 James Goode.
 */

#include "sqlite/detail/insert_query_string.hpp"

#include "json/json.hpp"
#include "json/serialisers/vector.hpp"
#include "json/serialisers/string.hpp"

std::string sqlite::detail::insert_query_string(
    std::string                               table,
    std::vector<boost::optional<const char*>> fields
    )
{
    int n_val_fields = std::count_if(
            fields.begin(), fields.end(),
            [](boost::optional<const char*> val) {
                return (bool)val;
            }
            );
    std::ostringstream query;
    query << "INSERT INTO " << table << "(";
    // Field names
    for( std::vector<boost::optional<const char*>>::iterator i =
            fields.begin();
         i != fields.end(); ++i )
    {
        if( (bool)*i )
        {
            query << **i;
            auto it2 = i;
            ++it2;
            if( it2 != fields.end() &&
                std::find_if(
                    it2,
                    fields.end(),
                    [](boost::optional<const char*> o) {
                        return (bool)o;
                    }
                    ) != fields.end() )
                query << ", ";
        }
    }

    query << ") VALUES (";
    // Bind value placeholders (?)
    std::vector<std::string> placeholders;
    std::fill_n(
            std::back_inserter(placeholders),
            n_val_fields,
            "?"
            );
    json::serialise(
            placeholders,
            (void (*)(const std::string&, std::ostream&))json::serialise,
            ", ",
            query
            );
    query << ")";
    return query.str();
}

