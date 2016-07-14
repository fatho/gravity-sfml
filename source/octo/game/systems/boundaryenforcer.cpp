#include "boundaryenforcer.hpp"

#include <cmath>
#include <octo/math/vector.hpp>

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
    const components::SpatialSnapshot& current = spatial.current();
    if (!std::isfinite(current.position.x) || !std::isfinite(current.position.y)) {
      // invalid coordinates indicate a programming error
      log.error("entity %d has non-finite coordinates {%f, %f}",
                entity.id().id(),
                current.position.x,
                current.position.y);
    } else if (math::vector::lengthSquared(current.position) > m_boundary * m_boundary) {
      // entity is out of bounds
      entity.destroy();
      // TODO: spawn fancy destruction effect
    }
  });
}
