cmake_minimum_required(VERSION 3.1)

project(histograms)
enable_language(CXX)

set(CMAKE_CXX_STANDARD 14) 
set(CMAKE_CXX_STANDARD_REQUIRED ON) 
set(CMAKE_CXX_EXTENSIONS OFF)


if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
   message(   "Clang is used" )
   set(CMAKE_CXX_FLAGS "-O2  -Wall -Wextra ")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
   message(   "gcc is used" )
   set(CMAKE_CXX_FLAGS "-O2  -march=native -Wall -Wextra -pedantic -Wshadow ")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
   message(   "intel is used" )
#elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
# using Visual Studio C++
else()
   message( FATAL_ERROR  "Not supported" )
endif()


include_directories(~/src/cxx-prettyprint)
include_directories(${PRETTYPRINT})
find_package(Boost)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(pdf_anlyzer  pdf_anlyzer.cpp)

