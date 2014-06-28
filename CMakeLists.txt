cmake_minimum_required(VERSION 2.6)
project(SQLite)

set(CMAKE_CXX_FLAGS "-std=c++0x")

include_directories(include)

# Allow the parent CMakeLists.txt to specify the location of the JSON library
# to save cloning the library as a submodule.
if(SQLITE_JSON_LIBRARY_LOCATION)
    include_directories(${SQLITE_JSON_LIBRARY_LOCATION}/include)
else()
    include_directories(json/include)
endif()

file(GLOB sources src/*.cpp src/detail/*.cpp)

add_library(
    sqlite
    ${sources}
    )

target_link_libraries(
    sqlite
    "sqlite3"
    )

