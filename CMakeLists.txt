cmake_minimum_required(VERSION 3.10)

project(MyProject)

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_STANDARD 11)

add_executable(server main.cpp
               ./timer/lst_timer.cpp
               ./http/http_conn.cpp
               ./log/log.cpp
               ./CGImysql/sql_connection_pool.cpp
               webserver.cpp
               config.cpp)

target_link_libraries(server pthread mysqlclient)

