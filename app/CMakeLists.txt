
find_package(SFML 2.3 COMPONENTS graphics window system)

set(SOURCES
  main.cpp
  octo/content/contentmanager.cpp
  octo/content/sfml.cpp
  octo/content/sfml/textureloader.cpp
  octo/game.cpp
  octo/gamestate.cpp)

add_executable(gravity ${SOURCES})
target_include_directories(gravity PRIVATE ${PROJECT_SOURCE_DIR} ${SFML_INCLUDE_DIR})
target_link_libraries(gravity fmtlog ${SFML_LIBRARIES})
