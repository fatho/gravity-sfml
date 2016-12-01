#pragma once

#include "../components/spatial.hpp"
#include "../components/dynamicbody.hpp"
#include "../events/entitycollision.hpp"
#include "../world.hpp"
#include <fmtlog/fmtlog.hpp>

#include <entityx/entityx.h>

#include <vector>

namespace octo {
namespace game {
namespace systems {

/*! \brief Responsible for performing collision response for rigid bodies.
 */
struct Bounce : public entityx::System<Bounce>, public entityx::Receiver<Bounce> {
  Bounce();

  /*! \brief Performs collision response calculations for all collisions that happened in the current frame.
   *  \param es The entity system involved.
   *  \param events (unused)
   *  \param dt (unused)
   */
  void update(entityx::EntityManager& es, entityx::EventManager& events, entityx::TimeDelta dt) override;

  /*! \brief Sets up event handlers.
   */
  void configure(entityx::EventManager& events) override;

  /*! \brief Receives collision events necessary for computing bouncing.
   */
  void receive(const events::EntityCollision& event);

private:
  /*! \brief Computes bouncing behavior for movable entities.
   *  \param collisionData The collision event to which the system needs to react.
   *  \todo Properly implement collision response.
   */
  static void bounce(events::EntityCollision& collisionData);

private:
  fmtlog::Log log = fmtlog::For<Bounce>();

  /// The collisions that occurred in the current frame.
  std::vector<events::EntityCollision> m_collisions;
};

}
}
}
