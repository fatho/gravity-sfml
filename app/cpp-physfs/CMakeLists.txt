cmake_minimum_required(VERSION 3.5)
project(cpp-physfs)

find_package(Boost 1.60 REQUIRED filesystem)
find_package(PhysFS REQUIRED)
find_package(SFML 2.3 COMPONENTS system)

set(SOURCES
  physfs.cpp)

add_library(cpp-physfs ${SOURCES})
target_include_directories(cpp-physfs PRIVATE
  ${Boost_INCLUDE_DIRS}
  ${SFML_INCLUDE_DIR}
  ${PHYSFS_INCLUDE_DIR})
target_link_libraries(cpp-physfs ${Boost_LIBRARIES} ${PHYSFS_LIBRARY} ${SFML_LIBRARIES})
