cmake_minimum_required(VERSION 3.5)
project(gravity-octo)

find_package(Boost 1.60 REQUIRED filesystem)
find_package(SFML 2.3 COMPONENTS graphics window audio system)
find_package(EntityX REQUIRED)

set(SOURCES
  game.cpp
  gamestate.cpp
  debugoverlay.cpp
  content/contentmanager.cpp
  content/filecontentmanager.cpp
  content/physfscontentmanager.cpp
  content/font.cpp
  content/music.cpp
  content/sfml.cpp
  content/shader.cpp
  game/world.cpp
  game/collision/mask.cpp
  game/components/attraction.cpp
  game/components/collisionmask.cpp
  game/components/dynamicbody.cpp
  game/components/planet.cpp
  game/components/spatial.cpp
  game/systems/attractionsystem.cpp
  game/systems/boundaryenforcer.cpp
  game/systems/debug.cpp
  game/systems/physics.cpp
  rendering/debugdraw.cpp
  states/ingamestate.cpp
  util/fpscounter.cpp
  )

add_library(octo ${SOURCES})
target_include_directories(octo PRIVATE
  ${SFML_INCLUDE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${ENTITYX_INCLUDE_DIR})
