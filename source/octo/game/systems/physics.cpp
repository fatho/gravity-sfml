#include "physics.hpp"

#include "../components.hpp"
#include <octo/math/all.hpp>

#include <cmath>

using namespace octo::game::systems;

Physics::Physics(World& world) : m_world(world) {}

void Physics::update(entityx::EntityManager& es, entityx::EventManager&, entityx::TimeDelta dt) {
  float timeStep = static_cast<float>(dt);
  integrate(es, timeStep);
}

void Physics::integrate(entityx::EntityManager& es, float timeStep) {
  using namespace components;
  es.each<Spatial, DynamicBody>([this, timeStep](entityx::Entity entity, Spatial& spatial, DynamicBody& body) {
    spatial.previous() = spatial.current();
    // using semi-implicit Euler

    // integrate linear motion
    body.linearMomentum += body.force * timeStep;
    sf::Vector2f displacement = body.velocity() * timeStep;
    // FIXME maybe put an upper limit to velocities, to prevent
    // if(math::vector::lengthSquared(displacement) >= math::util::sqr(m_maximumSpeed)) {
    //   displacement = math::vector::normalized(displacement) * m_maximumSpeed;
    //   log.debug("object too fast [%s]", entity.id());
    // }
    spatial.current().position += displacement;

    // integrate angular motion
    body.angularMomentum += body.torque * timeStep;
    spatial.current().rotationRadians() += body.angularVelocity() * timeStep;

    // reset accumulators
    body.force = sf::Vector2f();
    body.torque = 0;
  });
}
