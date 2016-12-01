cmake_minimum_required(VERSION 3.5)
project(gravity-octo)

find_package(Boost 1.62 REQUIRED filesystem system)
find_package(SFML 2.4 REQUIRED COMPONENTS graphics window audio system)
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
  game/collision/util.cpp
  game/components/attraction.cpp
  game/components/collisionmask.cpp
  game/components/dynamicbody.cpp
  game/components/health.cpp
  game/components/planet.cpp
  game/components/projectile.cpp
  game/components/spatial.cpp
  game/components/vessel.cpp
  game/events/componentmodified.cpp
  game/events/damage.cpp
  game/events/entitycollision.cpp
  game/events/explode.cpp
  game/systems/attractionsystem.cpp
  game/systems/boundaryenforcer.cpp
  game/systems/bounce.cpp
  game/systems/collision.cpp
  game/systems/debug.cpp
  game/systems/explosions.cpp
  game/systems/healthsystem.cpp
  game/systems/physics.cpp
  game/systems/projectiles.cpp
  rendering/debugdraw.cpp
  states/ingamestate.cpp
  util/fpscounter.cpp
  )

add_library(octo ${SOURCES})
target_include_directories(octo PRIVATE
  ${SFML_INCLUDE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${ENTITYX_INCLUDE_DIR})
