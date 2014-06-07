SQLite C++ API
==============

SQLite has a very straightforward C API, however, when a programs contains lots
of similar database structures, writing SQL queries can become quite tedious.
With C++ and some Boost libraries, we can remove some boilerplate code without
removing any power of SQL.

For example, take the following C code.

    sqlite3_prepare("SELECT id, name, location, .... FROM table");
    while( sqlite3_step(stmt) ) {
        output_struct.name     = sqlite3_get_column(1, stmt);
        output_struct.location = sqlite3_get_column(2, stmt);
        ...
    }

In C++, the structure may be defined as:

    struct output_struct_type : json::map_accessor
    {
        output_struct_type(json::object& o) :
            json::map_accessor(o)
        {
        }
        std::string& name() const { return get_string("name"); }
        std::string& location() const { return get_string("location"); }
    };

The struct can be adapted for use as a Boost Fusion container:

    BOOST_FUSION_ADAPT_STRUCT(
        output_struct_type,
        (std::string&, name())
        (std::string&, location())
        ...
        )

The SQL SELECT statement can now be executed using the select function in the SQLite library.

    json::list;
    sqlite::select<output_struct_type>(
        connection,
        "SELECT name, location FROM photograph ",
        boost::fusion::vector<>(),
        list
        );

Note that the list in this example is not typed.  Type information is taken
from the template parameter to sqlite::select to bind values from the query
result to the correct JSON object keys.

Now that the data types are defined, it is easy to insert data as well.

    sqlite::insert(
        "table_name",
        { "name", "location" },
        output_struct,
        connection
        );

The library also provides a function for devoid queries (queries that do not
produce a result).  Parameters can still be bound to placeholders in the query.

    sqlite::devoid(
        "DELETE FROM table_name WHERE location = ? ",
        boost::fusion::vector<std::string>("London"),
        connection
        );

