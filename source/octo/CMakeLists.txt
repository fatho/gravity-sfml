cmake_minimum_required(VERSION 3.5)
project(gravity-octo)

find_package(Boost 1.62 REQUIRED filesystem system)
find_package(SFML 2.4 REQUIRED COMPONENTS graphics window audio system)
find_package(EntityX REQUIRED)

# FIXME: find better way of referring to flatbuffers
add_library(flatbuffers STATIC IMPORTED)
set_target_properties(flatbuffers PROPERTIES
  IMPORTED_LOCATION "/usr/lib/libflatbuffers.a"
  INTERFACE_INCLUDE_DIRECTORIES "/usr/include")

# Process flat buffer schemas
find_program(FLATC flatc)

set(FLATBUFFER_FILES
  game/serialization/worldstate.fbs
  )

foreach(FBS ${FLATBUFFER_FILES})
  set(FBS_SRC_FILE "${PROJECT_SOURCE_DIR}/${FBS}")
  get_filename_component(FBS_OUT_DIR "${PROJECT_BINARY_DIR}/${FBS}" DIRECTORY)
  get_filename_component(FBS_OUT_FILE_BASE "${PROJECT_BINARY_DIR}/${FBS}" NAME_WE)
  set(FBS_OUT_FILE "${FBS_OUT_DIR}/${FBS_OUT_FILE_BASE}_generated.h")
  file(MAKE_DIRECTORY "${FBS_OUT_DIR}")
  add_custom_command(OUTPUT "${FBS_OUT_FILE}"
    COMMAND "${FLATC}" --cpp --scoped-enums --gen-object-api -o "${FBS_OUT_DIR}" "${FBS_SRC_FILE}"
    MAIN_DEPENDENCY "${FBS_SRC_FILE}")
  list(APPEND FLATBUFFER_GENERATED "${FBS_OUT_FILE}")
endforeach(FBS)

set(SOURCES
  debugoverlay.cpp
  debugoverlay.hpp
  game.cpp
  game.hpp
  gamestate.cpp
  gamestate.hpp

  content/contentmanager.cpp
  content/contentmanager.hpp
  content/filecontentmanager.cpp
  content/filecontentmanager.hpp
  content/font.cpp
  content/font.hpp
  content/music.cpp
  content/music.hpp
  content/physfscontentmanager.cpp
  content/physfscontentmanager.hpp
  content/sfml.cpp
  content/sfml.hpp
  content/shader.cpp
  content/shader.hpp
  content/streaming.hpp

  game/components.hpp
  game/systems.hpp
  game/world.cpp
  game/world.hpp

  game/collision/mask.cpp
  game/collision/mask.hpp
  game/collision/util.cpp
  game/collision/util.hpp

  game/components/attraction.cpp
  game/components/attraction.hpp
  game/components/collisionmask.cpp
  game/components/collisionmask.hpp
  game/components/debugdata.hpp
  game/components/dynamicbody.cpp
  game/components/dynamicbody.hpp
  game/components/health.cpp
  game/components/health.hpp
  game/components/planet.cpp
  game/components/planet.hpp
  game/components/projectile.cpp
  game/components/projectile.hpp
  game/components/spatial.cpp
  game/components/spatial.hpp
  game/components/vessel.cpp
  game/components/vessel.hpp

  game/events/componentmodified.cpp
  game/events/componentmodified.hpp
  game/events/damage.cpp
  game/events/damage.hpp
  game/events/entitycollision.cpp
  game/events/entitycollision.hpp
  game/events/explode.cpp
  game/events/explode.hpp

  game/systems/attractionsystem.cpp
  game/systems/attractionsystem.hpp
  game/systems/bounce.cpp
  game/systems/bounce.hpp
  game/systems/boundaryenforcer.cpp
  game/systems/boundaryenforcer.hpp
  game/systems/collision.cpp
  game/systems/collision.hpp
  game/systems/debug.cpp
  game/systems/debug.hpp
  game/systems/explosions.cpp
  game/systems/explosions.hpp
  game/systems/healthsystem.cpp
  game/systems/healthsystem.hpp
  game/systems/physics.cpp
  game/systems/physics.hpp
  game/systems/projectiles.cpp
  game/systems/projectiles.hpp

  math/all.hpp
  math/rect.hpp
  math/util.hpp
  math/vector.hpp

  rendering/debugdraw.cpp
  rendering/debugdraw.hpp

  states/ingamestate.cpp
  states/ingamestate.hpp

  util/fpscounter.cpp
  util/fpscounter.hpp
  util/interpolation.hpp
  util/pixelarray.hpp
  util/rectiterator.hpp
  # Dependencies on generated header files
  ${FLATBUFFER_GENERATED}
  )

add_library(octo ${SOURCES})
target_link_libraries(octo flatbuffers)
target_include_directories(octo PRIVATE
  ${SFML_INCLUDE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${ENTITYX_INCLUDE_DIR})
