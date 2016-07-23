#include "physics.hpp"

#include "../components.hpp"
#include <octo/math/rect.hpp>
#include <octo/math/vector.hpp>

#include <cmath>

using namespace octo::game::systems;

Physics::Physics(World& world) : m_world(world) {}

void Physics::update(entityx::EntityManager& es, entityx::EventManager&, entityx::TimeDelta dt) {
  float timeStep = static_cast<float>(dt);
  integrate(es, timeStep);
}

void Physics::integrate(entityx::EntityManager& es, float timeStep) {
  using namespace components;
  es.each<Spatial, DynamicBody>([timeStep](entityx::Entity, Spatial& spatial, DynamicBody& body) {
    spatial.previous() = spatial.current();
    // using semi-implicit Euler

    // integrate linear motion
    body.linearMomentum += body.force * timeStep;
    spatial.current().position += body.velocity() * timeStep;

    // integrate angular motion
    body.angularMomentum += body.torque * timeStep;
    spatial.current().rotationRadians() += body.angularVelocity() * timeStep;

    // reset accumulators
    body.force = sf::Vector2f();
    body.torque = 0;
  });
}
