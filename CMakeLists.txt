cmake_minimum_required(VERSION 3.10)

# Building server
project(cpp_server)

set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

find_package(Boost 1.65.1 REQUIRED COMPONENTS system)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(cpp_server run_server.cpp TCP_server.cpp TCP_server.hpp HTTP_connection.cpp HTTP_connection.hpp server_models.hpp pixel.h pixel.cpp time_date.h time_date.cpp server_models.cpp pixel_server.cpp pixel_server.hpp)
    target_link_libraries(cpp_server ${Boost_SYSTEM_LIBRARY})
endif()
