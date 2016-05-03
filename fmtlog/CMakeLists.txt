cmake_minimum_required(VERSION 3.5)
project(fmtlog)

set(SOURCES
  log.cpp)
  
find_package(Boost 1.60 REQUIRED)

add_library(fmtlog ${SOURCES})
target_include_directories(fmtlog PRIVATE ${Boost_INCLUDE_DIRS})