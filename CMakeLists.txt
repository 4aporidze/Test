cmake_minimum_required(VERSION 3.10)
project(VectorIteratorBoost CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Boost.Iterator is header-only, we just need include path
find_package(Boost REQUIRED)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()

add_executable(vector_iterator main.cpp)
