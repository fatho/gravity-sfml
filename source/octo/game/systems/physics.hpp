#pragma once

#include "../components/position.hpp"
#include "../components/dynamicbody.hpp"

#include <entityx/entityx.h>

namespace octo {
namespace game {
namespace systems {

/*! \brief Responsible for updating the moving parts of the simulation and performs physics related computations.
 *
 *  In this system, forces are converted into motion.
 */
struct Physics : public entityx::System<Physics> {
  /*! \brief Destroys all entities outside of the boundary.
   *
   *  This affects all entities with a \ref components::Position "Position" and \ref components::DynamicBody "DynamicBody" component.
   *  After computing the effect of a force, the corresponding data member of the \c DynamicBody component is reset to zero.
   *  It is expected that forces are recalculated every frame.
   *
   *  \param es the entity system involved,
   *  \param events (currently) unused,
   *  \param dt used in the semi-implicit Euler integration. Using a fixed time-step is advisable.
   */
  void update(entityx::EntityManager& es, entityx::EventManager& events, entityx::TimeDelta dt) override;
};

}
}
}
