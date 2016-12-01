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
  game/serialization/gamestate.fbs
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
  # TODO: add header files as well
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
  # Dependencies on generated header files
  ${FLATBUFFER_GENERATED}
  )

add_library(octo ${SOURCES})
target_link_libraries(octo flatbuffers)
target_include_directories(octo PRIVATE
  ${SFML_INCLUDE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${ENTITYX_INCLUDE_DIR})
