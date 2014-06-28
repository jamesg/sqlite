/*
 * SQLite - functions for simplifying SQLite queries in C++.
 * Copyright (C) 2014 James Goode.
 */

#include <iostream>

#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/vector.hpp>

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "json/map_accessor.hpp"

#include "sqlite/connection.hpp"
#include "sqlite/devoid.hpp"
#include "sqlite/insert.hpp"
#include "sqlite/select.hpp"

namespace
{
    struct photograph : json::map_accessor
    {
        photograph(json::object& o) :
            json::map_accessor(o)
        {
        }
        int& id() const { return get_int("id"); }
        std::string& title() const { return get_string("title"); }
    };

    void create_photograph_table(sqlite::connection& conn)
    {
        sqlite::devoid(
                "CREATE TABLE photograph ( "
                "photograph_id INTEGER PRIMARY KEY, "
                "title VARCHAR "
                ") ",
                boost::fusion::vector<>(),
                conn
                );
    }
}

BOOST_FUSION_ADAPT_STRUCT(
    photograph,
    (int, id())
    (std::string, title())
    )

SCENARIO("SELECT") {
    GIVEN("A photograph database") {
        sqlite::connection conn = sqlite::connection::in_memory_database();
        create_photograph_table(conn);
        WHEN("A photograph is inserted") {
            json::object photograph_o;
            photograph(photograph_o).title() = "Test";
            sqlite::insert(
                    "photograph",
                    { boost::optional<const char*>(), "title" },
                    photograph(photograph_o),
                    conn
                    );
            THEN("The photograph can be queried") {
                json::list out;
                sqlite::select<photograph>(
                        conn,
                        "SELECT photograph_id, title FROM photograph ",
                        boost::fusion::vector<>(),
                        out
                        );
                REQUIRE(out.size() == 1);
                REQUIRE(photograph(out[0]).title() == "Test");
            }
        }
    }
}

