#include "physics.hpp"

using namespace octo::game::systems;

void Physics::update(entityx::EntityManager& es, entityx::EventManager&, entityx::TimeDelta dt) {
  float timeStep = static_cast<float>(dt);
  integrate(es, timeStep);
  // TODO: collision detection & response
}

void Physics::integrate(entityx::EntityManager& es, float timeStep) {
  using namespace components;
  es.each<Spatial, DynamicBody>(
    [timeStep](entityx::Entity, Spatial& spatial, DynamicBody& body) {
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

void Physics::collisionResponse(entityx::EntityManager& es, float timeStep) {
}
