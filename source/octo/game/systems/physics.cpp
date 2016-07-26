#include "physics.hpp"

#include "../components.hpp"
#include <octo/math/all.hpp>

#include <boost/math/constants/constants.hpp>

#include <cmath>

using namespace octo::game::systems;

Physics::Physics(World& world) : m_world(world) {}

void Physics::update(entityx::EntityManager& es, entityx::EventManager&, entityx::TimeDelta dt) {
  float timeStep = static_cast<float>(dt);
  integrate(es, timeStep);
}

void Physics::integrate(entityx::EntityManager& es, float timeStep) {
  using namespace components;
  es.each<Spatial, DynamicBody>(
      [this, timeStep](entityx::Entity entity, Spatial& spatial, DynamicBody& body) {
        spatial.previous() = spatial.current();
        // using semi-implicit Euler

        if (!body.sleeping) {
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
          float rotation = body.angularVelocity() * timeStep;
          spatial.current().rotationRadians() += rotation;

          // auto sleep
          // if (math::vector::lengthSquared(displacement) < 1 &&
          //     std::abs(rotation) < boost::math::constants::pi<float>() / 8 &&
          //     math::vector::lengthSquared(body.force) < 1 &&
          //     std::abs(body.torque) < body.inertia) {
          //   body.sleeping = true;
          // }
        }

        // reset accumulators
        body.force = sf::Vector2f();
        body.torque = 0;
      });
}
