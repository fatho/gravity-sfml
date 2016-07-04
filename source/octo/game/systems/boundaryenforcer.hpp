#pragma once

#include "../components/spatial.hpp"

#include <entityx/entityx.h>

namespace octo {
namespace game {
namespace systems {

/*! \brief This system removes all entities moving outside a configurable range around the origin.
 */
struct BoundaryEnforcer : public entityx::System<BoundaryEnforcer> {

  /*! \brief Sets an initial boundary radius.
   *  \param boundaryRadius the initial boundary radius.
   */
  BoundaryEnforcer(float boundaryRadius);

  /// sets the radius of the boundary beyond which entities are removed
  void setBoundaryRadius(float radius);

  /// returns the current boundary radius
  float boundaryRadius() const;

  /*! \brief Destroys all entities outside of the boundary.
   *
   *  This affects all entities with a \ref components::Spatial component.
   *
   *  \param es the entity system involved,
   *  \param events (currently) unused,
   *  \param dt unused.
   *
   *  \todo define and raise event for destroyed entities in order to react with some effects
   *  \todo some component to opt-out from destruction might be useful
   */
  void update(entityx::EntityManager& es, entityx::EventManager& events, entityx::TimeDelta dt) override;

private:
  /// The boundary radius.
  float m_boundary;
};
}
}
}
