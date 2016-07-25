#include "projectiles.hpp"

#include "../components.hpp"
#include "../events/explode.hpp"

#include <algorithm>
#include <cassert>
#include <functional>

namespace octo {
namespace game {
namespace systems {

Projectiles::Projectiles() {}

void Projectiles::configure(entityx::EventManager& events) {
  events.subscribe<events::EntityCollision>(*this);
}

void Projectiles::update(entityx::EntityManager& es, entityx::EventManager& events,
                         entityx::TimeDelta dt) {
  for (auto& hit : m_projectileHits) {
    if (hit.entities[0].valid()) {
      // TODO put "bounce" code in separate method
      auto spatial = hit.entities[0].component<components::Spatial>();
      auto body = hit.entities[0].component<components::DynamicBody>();
      assert(spatial.valid());
      assert(body.valid());
      sf::Vector2f lcont = spatial->current().globalToLocal(false).transformPoint(hit.contactPoint);
      // FIXME add momentum of other body
      sf::Vector2f contactMomentum = body->momentumAt(lcont);
      float dir = math::vector::dot(contactMomentum, hit.normals[1]);
      sf::Vector2f normalMomentum = hit.normals[1] * dir;

      if(dir < 0) {
        // only apply impulse when not moving outwards already
        sf::Vector2f tangentMomentum = contactMomentum - normalMomentum;
        float restitution = 0.8f;
        float friction = 0.1f;
        body->applyLinearImpulse(lcont, - normalMomentum * (1.f + restitution) - tangentMomentum * friction);
      }
      // prevent tunneling through planets
      float forceDir = math::vector::dot(body->force, hit.normals[1]);
      if(forceDir < 0) {
        body->applyForce(lcont, - forceDir * hit.normals[1]);
      }

      //triggerProjectile(events, hit.entities[0]);

      // TODO apply direct damage to `hit.entities[1]` (due to being hit)

      // destroy the projectile
      //hit.entities[0].destroy();
    }
  }
  m_projectileHits.clear();

  for (auto& coll : m_projectileCollisions) {
    for (auto& e : coll.entities) {
      if (e.valid()) {
        triggerProjectile(events, e);
        e.destroy();
      }
    }
  }
  m_projectileCollisions.clear();
}

void Projectiles::receive(const events::EntityCollision& event) {
  bool isProjectile[2];
  std::transform(begin(event.entities), end(event.entities), isProjectile, Projectiles::isProjectile);
  // only care for collisions if at least one of the entities is a projectile
  if (isProjectile[0] && isProjectile[1]) {
    m_projectileCollisions.push_back(event);
  } else if (isProjectile[0]) {
    m_projectileHits.push_back(event);
  } else if (isProjectile[1]) {
    m_projectileHits.push_back(event);
    m_projectileHits.back().swapOrder();
  }
}

bool Projectiles::isProjectile(entityx::Entity entity) {
  return entity.has_component<components::Projectile>();
}

void Projectiles::triggerProjectile(entityx::EventManager& events,
                                    entityx::Entity projectileEntity) {
  auto spatial = projectileEntity.component<components::Spatial>();
  auto projectile = projectileEntity.component<components::Projectile>();
  if (spatial && projectile) {
    log.debug("triggering projectile %s", projectileEntity);
    // TODO replace with scriptable effects
    events.emit<events::Explode>(spatial->current().position, projectile->explosionRadius, 1, 1);
  } else {
    log.error("non-projectile %s cannot be triggered", projectileEntity);
  }
}
}
}
}
