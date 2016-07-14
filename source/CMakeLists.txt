cmake_minimum_required(VERSION 3.5)
project(gravity-bin)

find_package(Boost 1.60 REQUIRED filesystem)
find_package(SFML 2.3 COMPONENTS graphics window audio system)
find_package(EntityX REQUIRED)

# use root source folder as include path
include_directories(${PROJECT_SOURCE_DIR})

# build sub components
add_subdirectory(fmtlog)
add_subdirectory(cpp-physfs)
add_subdirectory(octo)

add_executable(gravity main.cpp)
target_include_directories(gravity PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${SFML_INCLUDE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${ENTITYX_INCLUDE_DIR})
target_link_libraries(gravity
  # integrated libraries
  octo
  fmtlog
  cpp-physfs
  # third party libraries
  ${SFML_LIBRARIES}
  ${Boost_LIBRARIES}
  ${ENTITYX_LIBRARY})
