#include "projectiles.hpp"

#include "../components.hpp"
#include "../events/explode.hpp"

#include <algorithm>
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
    if (hit.entity[0].valid()) {
      triggerProjectile(events, hit.entity[0]);

      // TODO apply direct damage to `hit.entity[1]` (due to being hit)

      // destroy the projectile
      hit.entity[0].destroy();
    }
  }
  m_projectileHits.clear();

  for (auto& coll : m_projectileCollisions) {
    for (auto& e : coll.entity) {
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
  std::transform(begin(event.entity), end(event.entity), isProjectile, Projectiles::isProjectile);
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
