cmake_minimum_required(VERSION 3.24)
project(cppunittest CXX)

set(CMAKE_CXX_STANDARD            23)
set(CMAKE_CXX_STANDARD_REQUIRED   ON)
set(CMAKE_CXX_EXTENSIONS          OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(cppunittest STATIC)
target_sources(cppunittest
    PRIVATE src/cppunittest/unittest.cpp
    PRIVATE src/cppunittest/unittest.hpp
    PRIVATE src/cppunittest/driver.cpp)
target_include_directories(cppunittest PUBLIC src)

if (MSVC)
    target_compile_options(cppunittest PRIVATE "/W4")
else ()
    target_compile_options(cppunittest PRIVATE "-Wall" "-Wextra" "-Wpedantic")
endif ()

# Make cppunittest CMake scripts visible to the parent scope.
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/CMake")
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" PARENT_SCOPE)
