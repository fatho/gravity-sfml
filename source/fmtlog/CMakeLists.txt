cmake_minimum_required(VERSION 3.5)
project(fmtlog)

set(SOURCES
  fmtlog.cpp
  fmtlog.hpp)

find_package(Boost 1.62 REQUIRED)

add_library(fmtlog ${SOURCES})
target_include_directories(fmtlog PRIVATE ${Boost_INCLUDE_DIRS})
