#pragma once

#include "../components/spatial.hpp"
#include "../components/dynamicbody.hpp"
#include "../events/entitycollision.hpp"
#include "../world.hpp"

#include <fmtlog/fmtlog.hpp>

#include <entityx/entityx.h>

namespace octo {
namespace game {
namespace systems {

/*! \brief This system detects collisions between entities.
 */
struct Collision : public entityx::System<Collision> {
  Collision(World& world);

  /*! \brief Detects collisions and raises the corresponding \ref events::EntityCollision events.
   */
  void update(entityx::EntityManager& es, entityx::EventManager& events, entityx::TimeDelta dt) override;

private:
  fmtlog::Log log = fmtlog::For<Collision>();
  World& m_world;
  int m_normalAccuracy = 4;
};

}
}
}
