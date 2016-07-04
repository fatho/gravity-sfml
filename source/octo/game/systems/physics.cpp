#include "physics.hpp"

using namespace octo::game::systems;

void Physics::update(entityx::EntityManager& es, entityx::EventManager&, entityx::TimeDelta dt) {
  using namespace octo::game::components;
  float dtf = static_cast<float>(dt);
  es.each<Spatial, DynamicBody>([dtf](entityx::Entity, Spatial& spatial, DynamicBody& body) {
      // using semi-implicit euler

      // integrate linear motion
      body.linearMomentum += body.force * dtf;
      spatial.position += body.velocity() * dtf;

      // integrate angular motion
      body.angularMomentum += body.torque * dtf;
      spatial.rotationRadians() += body.angularVelocity() * dtf;

      // reset accumulators
      body.force = sf::Vector2f();
      body.torque = 0;
    });
  // TODO: collision detection & response
}
