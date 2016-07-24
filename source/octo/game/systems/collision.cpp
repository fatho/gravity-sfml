#include "collision.hpp"

#include "../collision/util.hpp"
#include "../components.hpp"
#include "../events/entitycollision.hpp"
#include <octo/math/rect.hpp>
#include <octo/math/vector.hpp>
#include <octo/util/rectiterator.hpp>

#include <cmath>

using namespace octo::game::systems;

Collision::Collision(World& world) : m_world(world) {}

void Collision::update(entityx::EntityManager& es, entityx::EventManager& events,
                       entityx::TimeDelta dt) {
  using namespace components;
  es.each<Spatial, CollisionMask>(
      [&](entityx::Entity entityA, Spatial& spatialA, CollisionMask& maskA) {
        es.each<Spatial, CollisionMask>([&](
            entityx::Entity entityB, Spatial& spatialB, CollisionMask& maskB) {
          // use artificial order to only process every pair once
          if (entityA < entityB) {
            // setup transformation from A's pixels to B's pixels
            sf::Transform localToWorldB = collision::maskToGlobal(spatialB.current(), maskB);

            sf::Transform atob = collision::maskToGlobal(spatialA.current(), maskA) *
                collision::globalToMask(spatialB.current(), maskB);

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
              sf::Vector2f contactPoint{0, 0};
              size_t numContacts = 0;
              for(auto& bpos : util::rectRange(area)) {
                if (maskB.mask.at(bpos.x, bpos.y) != collision::Pixel::NoCollision) {
                  auto apos = math::vector::map(btoa.transformPoint(bpos.x, bpos.y), [](float x) {
                      return static_cast<size_t>(std::round(x));
                    });
                  if (pixRectA.contains(apos) &&
                      maskA.mask.at(apos.x, apos.y) != collision::Pixel::NoCollision) {
                    contactPoint += localToWorldB.transformPoint(bpos.x, bpos.y);
                    numContacts += 1;
                  }
                }
              }
              // if there was a collision, compute contact
              if (numContacts > 0) {
                contactPoint /= (float)numContacts;
                // TODO: compute collision normal
                log.debug("collision [%s] and [%s] at (%.1f, %.1f)",
                          entityA.id(),
                          entityB.id(),
                          contactPoint.x,
                          contactPoint.y);
                events.emit(events::EntityCollision{{entityA, entityB}, contactPoint});
              }
            }
          }
        });
      });
}
