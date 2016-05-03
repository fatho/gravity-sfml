
find_package(Boost 1.60 REQUIRED filesystem)
find_package(SFML 2.3 COMPONENTS graphics window audio system)

set(SOURCES
  main.cpp
  octo/content/contentmanager.cpp
  octo/content/filecontentmanager.cpp
  octo/content/music.cpp
  octo/content/sfml.cpp
  octo/content/shader.cpp
  octo/game.cpp
  octo/gamestate.cpp)

add_executable(gravity ${SOURCES})
target_include_directories(gravity PRIVATE ${PROJECT_SOURCE_DIR} ${SFML_INCLUDE_DIR} ${Boost_INCLUDE_DIRS})
target_link_libraries(gravity fmtlog ${SFML_LIBRARIES} ${Boost_LIBRARIES})
