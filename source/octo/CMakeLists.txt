cmake_minimum_required(VERSION 3.5)
project(gravity-octo)

set(SOURCES
  content/contentmanager.cpp
  content/filecontentmanager.cpp
  content/physfscontentmanager.cpp
  content/music.cpp
  content/sfml.cpp
  content/shader.cpp
  game.cpp
  gamestate.cpp
  game/world.cpp
  game/components/attraction.cpp
  game/components/dynamicbody.cpp
  game/components/planet.cpp
  game/components/spatial.cpp
  game/systems/attractionsystem.cpp
  game/systems/boundaryenforcer.cpp
  game/systems/physics.cpp
  rendering/debugdraw.cpp
  states/ingamestate.cpp
  )

add_library(octo ${SOURCES})
target_include_directories(octo PRIVATE
  ${SFML_INCLUDE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${ENTITYX_INCLUDE_DIR}
  ${BOX2D_INCLUDE_DIRS}
  )
