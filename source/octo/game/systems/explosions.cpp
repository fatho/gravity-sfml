#include "explosions.hpp"

#include <octo/game/collision/util.hpp>
#include <octo/game/events/componentmodified.hpp>
#include <octo/game/components.hpp>
#include <octo/math/all.hpp>
#include <octo/util/rectiterator.hpp>

#include <SFML/Graphics/Rect.hpp>

#include <algorithm>
#include <functional>

namespace octo {
namespace game {
namespace systems {

Explosions::Explosions() {}

void Explosions::configure(entityx::EventManager& events) {
  events.subscribe<events::Explode>(*this);
}

void Explosions::update(entityx::EntityManager& es, entityx::EventManager& events,
                        entityx::TimeDelta dt) {
  for (auto& explosion : m_explosions) {
    log.debug("explosion at {%f, %f}", explosion.center.x, explosion.center.y);
    sf::FloatRect aabb =
        math::rect::fromCenterSize(explosion.center, {explosion.radius * 2, explosion.radius * 2});
    float explosionRadiusSq = explosion.radius * explosion.radius;
    collision::aabbQuery(
        es, aabb, [&](entityx::Entity hit, const collision::AabbQueryData& result) {
          log.debug("explosion hit [%s]", hit.id());
          // transform explosion center to local coordinates once
          sf::Vector2f localExplosionCenter = result.globalToMask.transformPoint(explosion.center);
          bool maskChanged = false;
          for (auto& pos : util::rectRange(result.intersectionMask)) {
            // the previous transform involved no scaling
            if (math::vector::lengthSquared(localExplosionCenter -
                                            math::vector::vector_cast<float>(pos))
                <= explosionRadiusSq) {
              if(result.collisionComponent.mask.at(pos.x, pos.y) == collision::Pixel::SolidDestructible) {
                result.collisionComponent.mask.at(pos.x, pos.y) = collision::Pixel::NoCollision;
                maskChanged = true;
              }
            }
          }
          if(maskChanged) {
            events.emit<events::ComponentModified<components::CollisionMask>>(hit);
          }
          // TODO spawn debris
        });
  }
  m_explosions.clear();
}

void Explosions::receive(const events::Explode& event) {
  m_explosions.push_back(event);
}
}
}
}
