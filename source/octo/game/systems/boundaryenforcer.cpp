#include "boundaryenforcer.hpp"

#include <cmath>
#include <more-math/vector.hpp>

using namespace octo::game::systems;

BoundaryEnforcer::BoundaryEnforcer(float boundaryRadius) : m_boundary(boundaryRadius) {}

void BoundaryEnforcer::setBoundaryRadius(float radius) {
  m_boundary = radius;
}
float BoundaryEnforcer::boundaryRadius() const {
  return m_boundary;
}

void BoundaryEnforcer::update(entityx::EntityManager& es, entityx::EventManager&,
                              entityx::TimeDelta) {
  es.each<components::Spatial>([&](entityx::Entity entity, components::Spatial& spatial) {
      // kill every entity that is out of bounds or has invalid coordinates
      if (!std::isfinite(spatial.position.x) || !std::isfinite(spatial.position.y) ||
          math::vector::lengthSquared(spatial.position) > m_boundary * m_boundary) {
        entity.destroy();
        // TODO: spawn fancy destruction effect
      }
    });
}
