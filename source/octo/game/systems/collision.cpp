#include "collision.hpp"

#include "../components.hpp"
#include "../events/entitycollision.hpp"
#include <octo/math/rect.hpp>
#include <octo/math/vector.hpp>

#include <cmath>

using namespace octo::game::systems;

Collision::Collision(World& world) : m_world(world) {}

void Collision::update(entityx::EntityManager& es, entityx::EventManager& events, entityx::TimeDelta dt) {
  using namespace components;
  es.each<Spatial, CollisionMask>(
      [&](entityx::Entity entityA, Spatial& spatialA, CollisionMask& maskA) {
        es.each<Spatial, CollisionMask>([&](
            entityx::Entity entityB, Spatial& spatialB, CollisionMask& maskB) {
          // use artificial order to only process every pair once
          if (entityA < entityB) {
            // setup transformation from A's pixels to B's pixels
            sf::Transform localToWorldA;
            localToWorldA.translate(-0.5f * maskA.size() + maskA.anchor); // pixel A -> local A
            localToWorldA.rotate(spatialA.current().rotationDegrees);     // ... -> local rotated A
            localToWorldA.translate(spatialA.current().position);         // ... -> global

            sf::Transform localToWorldB;
            localToWorldB.translate(-0.5f * maskB.size() + maskB.anchor); // pixel A -> local A
            localToWorldB.rotate(spatialB.current().rotationDegrees);     // ... -> local rotated A
            localToWorldB.translate(spatialB.current().position);         // ... -> global

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
              // compute average of colliding pixels
              sf::Vector2f contactPoint {0, 0};
              size_t numContacts = 0;
              for (size_t by = area.top; by < area.top + area.height; ++by) {
                for (size_t bx = area.left; bx < area.left + area.width; ++bx) {
                  if (maskB.mask.at(bx, by) != collision::Pixel::NoCollision) {
                    auto apos = math::vector::map(btoa.transformPoint(bx, by), [](float x) {
                      return static_cast<size_t>(std::round(x));
                    });
                    if (pixRectA.contains(apos) &&
                        maskA.mask.at(apos.x, apos.y) != collision::Pixel::NoCollision) {
                      contactPoint += localToWorldB.transformPoint(bx, by);
                      numContacts += 1;
                    }
                  }
                }
              }
              // if there was a collision, compute contact
              if(numContacts > 0) {
                contactPoint /= (float)numContacts;
                // TODO: compute collision normal
                log.debug("collision [%s] and [%s] at (%.1f, %.1f)", entityA.id(), entityB.id(), contactPoint.x, contactPoint.y);
                events.emit(events::EntityCollision { { entityA, entityB }, contactPoint });
              }
            }
          }
        });
      });
}
