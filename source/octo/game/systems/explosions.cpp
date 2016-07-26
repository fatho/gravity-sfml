#include "explosions.hpp"

#include <octo/game/collision/util.hpp>
#include <octo/game/events/componentmodified.hpp>
#include <octo/game/events/damage.hpp>
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
    float queryRadius = std::max(explosion.damageRadius, explosion.destructionRadius);
    sf::FloatRect aabb =
        math::rect::fromCenterSize(explosion.center, {queryRadius * 2, queryRadius * 2});
    float destructionRadiusSq = math::util::sqr(explosion.destructionRadius);
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
                <= destructionRadiusSq) {
              if(result.collisionComponent.mask.at(pos.x, pos.y) == collision::Pixel::SolidDestructible) {
                result.collisionComponent.mask.at(pos.x, pos.y) = collision::Pixel::NoCollision;
                maskChanged = true;
              }
            }
          }
          if(maskChanged) {
            events.emit<events::ComponentModified<components::CollisionMask>>(hit);
          }
          // TODO maybe make range for applying force larger
          auto body = hit.component<components::DynamicBody>();
          if(body.valid()) {
            sf::Vector2f r = result.spatialComponent.current().position - explosion.center;
            float lenSq = math::vector::lengthSquared(r);
            if(lenSq < math::util::sqr(explosion.damageRadius)) {
              float len = std::sqrt(lenSq);
              float fscale = 1 - std::sqrt(lenSq) / explosion.damageRadius;
              sf::Vector2f forceDir = r / std::max(1.f, len);
              // close enough to apply damage and impulse
              sf::Vector2f impulse = forceDir * explosion.force * fscale;
              body->linearMomentum += impulse;
              log.debug("applying explosive force %.1f %.1f", impulse.x, impulse.y);
              events.emit(events::Damage { hit, fscale * explosion.damage });
            }
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
