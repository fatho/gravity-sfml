#pragma once

#include "../components/position.hpp"

#include <entityx/entityx.h>

namespace octo {
namespace game {
namespace systems {

/*! \brief This system removes all entities moving outside a configurable range around the origin.
 */
struct BoundaryEnforcer : public entityx::System<BoundaryEnforcer> {

  BoundaryEnforcer(float boundaryRadius);

  /// sets the radius of the boundary beyond which entities are removed
  void setBoundaryRadius(float radius);

  /// returns the current boundary radius
  float boundaryRadius() const;

  void update(entityx::EntityManager& es, entityx::EventManager&, entityx::TimeDelta dt) override;

private:
  float m_boundary;
};
}
}
}
