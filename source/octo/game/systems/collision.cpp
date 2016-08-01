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
  es.each<Spatial, CollisionMask>([&](
      entityx::Entity entityA, Spatial& spatialA, CollisionMask& maskA) {
    es.each<Spatial, CollisionMask>([&](
        entityx::Entity entityB, Spatial& spatialB, CollisionMask& maskB) {
      // use artificial order to only process every pair once
      if (entityA < entityB && (maskA.selector & maskB.selector) != 0) {
        // setup transformation from A's pixels to B's pixels
        sf::Transform localToWorldB = collision::maskToGlobal(spatialB.current(), maskB);

        sf::Transform atob = collision::globalToMask(spatialB.current(), maskB) *
                             collision::maskToGlobal(spatialA.current(), maskA);

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
          for (auto& bpos : util::rectRange(area)) {
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
            contactPoint /= static_cast<float>(numContacts);
            sf::Vector2f contactA =
                collision::globalToMask(spatialA.current(), maskA).transformPoint(contactPoint);
            sf::Vector2f contactB =
                collision::globalToMask(spatialB.current(), maskB).transformPoint(contactPoint);
            sf::Vector2f normalA = math::vector::rotate(
                spatialA.current().rotationRadians(),
                collision::computeNormal(maskA.mask, 3, contactA.x, contactA.y));
            sf::Vector2f normalB = math::vector::rotate(
                spatialB.current().rotationRadians(),
                collision::computeNormal(maskB.mask, 3, contactB.x, contactB.y));
            log.debug(
                "collision [%s] and [%s] at (%.1f, %.1f); normals (%.2f, %.2f) and (%.2f, %.2f)",
                entityA.id(),
                entityB.id(),
                contactPoint.x,
                contactPoint.y,
                normalA.x,
                normalA.y,
                normalB.x,
                normalB.y);
            events::EntityCollision collisionData(
                {entityA, entityB}, {normalA, normalB}, contactPoint);
            if (!maskA.sensor && !maskB.sensor) {
              bounce(collisionData);
            }
            events.emit(collisionData);
          }
        }
      }
    });
  });
}

void Collision::bounce(events::EntityCollision& colData) {
  std::array<entityx::ComponentHandle<components::Spatial>, 2> spatials;
  std::array<entityx::ComponentHandle<components::DynamicBody>, 2> bodies;
  std::array<sf::Vector2f, 2> lcont;
  std::array<sf::Vector2f, 2> contactMomenta;
  sf::Vector2f contactMomentum;
  int divisor = 0;
  for (int i = 0; i < 2; ++i) {
    spatials[i] = colData.entities[i].component<components::Spatial>();
    bodies[i] = colData.entities[i].component<components::DynamicBody>();
    lcont[i] = spatials[i]->current().globalToLocal(false).transformPoint(colData.contactPoint);
    if (bodies[i].valid()) {
      contactMomenta[i] = bodies[i]->momentumAt(lcont[i]);
      contactMomentum += contactMomenta[i];
      divisor += 1;
    }
  }
  if (divisor == 0) {
    return;
  }
  for (int cur = 0; cur <= 1; ++cur) {
    int other = 1 - cur;
    if (spatials[cur].valid() && bodies[cur].valid()) {
      float dir = math::vector::dot(contactMomentum, colData.normals[other]);
      sf::Vector2f normalMomentum = colData.normals[other] * dir;

      if (dir < 0) {
        // only apply impulse when not moving outwards already
        sf::Vector2f tangentMomentum = contactMomentum - normalMomentum;
        float restitution = 0.8f;
        float friction = 0.1f;
        bodies[cur]->applyLinearImpulse(lcont[cur],
                                        -normalMomentum * (1.f + restitution) /
                                                static_cast<float>(divisor) -
                                            tangentMomentum * friction,
                                        false);
      }
      // prevent tunneling through planets
      float forceDir = math::vector::dot(bodies[cur]->force, colData.normals[other]);
      if (forceDir < 0) {
        bodies[cur]->applyForce(lcont[cur], -forceDir * colData.normals[other], false);
      }
    }
  }
}
