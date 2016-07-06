cmake_minimum_required(VERSION 3.5)
project(gravity-sfml)

# export compilation database, containing compile commands for each file (used for auto-completion)
set(CMAKE_EXPORT_COMPILE_COMMANDS on)

# enable C++14 standard and disable GNU extensions (i.e. use -std=c++14 instead of -std=gnu++14)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_EXTENSIONS off)

if(MSVC)
  # Force to always compile with W4
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-unused-parameter -pedantic")
endif()

# might add assets/tools/etc. here later
add_subdirectory(assets)
# the actual game application
add_subdirectory(source)
# documentation
add_subdirectory(documentation)
