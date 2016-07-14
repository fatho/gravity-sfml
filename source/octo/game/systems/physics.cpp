#include "physics.hpp"

#include "../components.hpp"
#include <octo/math/rect.hpp>
#include <octo/math/vector.hpp>

#include <cmath>

using namespace octo::game::systems;

void Physics::update(entityx::EntityManager& es, entityx::EventManager&, entityx::TimeDelta dt) {
  float timeStep = static_cast<float>(dt);
  integrate(es, timeStep);
  collisionDetection(es, timeStep);
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

void Physics::collisionDetection(entityx::EntityManager& es, float timeStep) {
  using namespace components;
  es.each<Spatial, CollisionMask>(
      [&](entityx::Entity entityA, Spatial& spatialA, CollisionMask& maskA) {
        es.each<Spatial, CollisionMask>([&](
            entityx::Entity entityB, Spatial& spatialB, CollisionMask& maskB) {
          // use artificial order to only process every pair once
          if (entityA < entityB) {
            // setup transformation from A's pixels to B's pixels
            sf::Transform atob;
            atob.translate(-0.5f * maskA.size() + maskA.anchor); // pixel A -> local A
            atob.rotate(spatialA.current().rotationDegrees);     // ... -> local rotated A
            atob.translate(spatialA.current().position);         // ... -> global
            atob.translate(-spatialB.current().position);        // ... -> local rotated B
            atob.rotate(-spatialB.current().rotationDegrees);    // ... -> local B
            atob.translate(0.5f * maskB.size() - maskB.anchor);  // ... -> pixel B

            sf::Transform btoa{atob.getInverse()};

            // first check AABB
            sf::Rect<size_t> pixRectA{{0, 0}, maskA.mask.size()};
            sf::Rect<size_t> pixRectB{{0, 0}, maskB.mask.size()};
            sf::FloatRect pixRectAtoB = atob.transformRect(math::rect::rect_cast<float>(pixRectA));
            sf::FloatRect intersection;
            if (pixRectAtoB.intersects(math::rect::rect_cast<float>(pixRectB), intersection)) {
              // if AABBs intersect, check pixels
              sf::Rect<size_t> area = math::rect::integralOutwards<size_t>(intersection);
              for (size_t by = area.top; by < area.top + area.height; ++by) {
                for (size_t bx = area.left; bx < area.left + area.width; ++bx) {
                  if (maskB.mask.at(bx, by) != collision::Pixel::NoCollision) {
                    auto apos = math::vector::map(btoa.transformPoint(bx, by), [](float x) {
                      return static_cast<size_t>(std::round(x));
                    });
                    if (pixRectA.contains(apos) &&
                        maskA.mask.at(apos.x, apos.y) != collision::Pixel::NoCollision) {
                      // TODO: collision response
                      return;
                    }
                  }
                }
              }
            }
          }
        });
      });
}
