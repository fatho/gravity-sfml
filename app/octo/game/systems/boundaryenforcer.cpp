#include "boundaryenforcer.hpp"

#include <cmath>
#include <math/vector.hpp>

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
  es.each<components::Position>([&](entityx::Entity entity, components::Position& pos) {
    if (!std::isfinite(pos.position.x) || !std::isfinite(pos.position.y) ||
        math::vector::lengthSquared(pos.position) > m_boundary * m_boundary) {
      entity.destroy();
      // TODO: spawn fancy destruction effect
    }
  });
}
