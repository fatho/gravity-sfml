#pragma once

#include "../components/spatial.hpp"
#include "../components/dynamicbody.hpp"

#include <entityx/entityx.h>
#include <Box2D/Dynamics/b2World.h>

namespace octo {
namespace game {
namespace systems {

/*! \brief Responsible for updating the moving parts of the simulation and performs physics related computations.
 *
 *  In this system, forces are converted into motion using a semi-implicit Euler integration.
 */
class Physics : public entityx::System<Physics> {
public:
  Physics();

  /*! \brief Destroys all entities outside of the boundary.
   *
   *  This affects all entities with \ref components::DynamicBody "DynamicBody" component.
   *  After computing the effect of a force, the corresponding data member of the
   *  \c DynamicBody component is reset to zero, since it is expected that forces are
   *  recalculated every frame.
   *
   *  Note that this does not modify the \ref components::Spatial "Spatial" component,
   *  which is eventually used for rendering. It is the responsibility of the game loop
   *  to interpolate the position from the previous and current state.
   *
   *  \param es the entity system involved,
   *  \param events (currently) unused,
   *  \param dt used in the semi-implicit Euler integration. Using a fixed time-step is advisable.
   */
  void update(entityx::EntityManager& es, entityx::EventManager& events, entityx::TimeDelta dt) override;


  b2World& engine() {
    return m_b2world;
  }

  const b2World& engine() const {
    return m_b2world;
  }

private:
  b2World m_b2world;
};

}
}
}
