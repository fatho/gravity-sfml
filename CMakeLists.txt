cmake_minimum_required(VERSION 3.5)
project(gravity-sfml)

# export compilation database, containing compile commands for each file (used for auto-completion)
set(CMAKE_EXPORT_COMPILE_COMMANDS on)

# enable C++14 standard and disable GNU extensions (i.e. use -std=c++14 instead of -std=gnu++14)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_EXTENSIONS off)

# might add assets/tools/etc. here later
add_subdirectory(fmtlog)
add_subdirectory(cpp-physfs)
add_subdirectory(assets)
# the actual game application
add_subdirectory(app)
