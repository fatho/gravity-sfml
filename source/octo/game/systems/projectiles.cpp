#include "projectiles.hpp"

#include "../components.hpp"

#include <algorithm>

namespace octo {
namespace game {
namespace systems {

Projectiles::Projectiles() {}

void Projectiles::configure(entityx::EventManager& events) {
  events.subscribe<events::EntityCollision>(*this);
}

void Projectiles::update(entityx::EntityManager& es, entityx::EventManager& events, entityx::TimeDelta dt) {
  
}

void Projectiles::receive(const events::EntityCollision& event) {
  bool isProjectile[2];
  std::transform(event.entity, event.entity + 2, isProjectile, Projectiles::isProjectile);
  // only care for collisions if at least one of the entities is a projectile
  if(isProjectile[0] && isProjectile[1]) {
    interProjectileCollision(event);
  } else if(isProjectile[0]) {
    projectileHit(event.entity[0], event.entity[1]);
  } else if(isProjectile[1]) {
    projectileHit(event.entity[1], event.entity[0]);
  }
}

bool Projectiles::isProjectile(entityx::Entity entity) {
  return entity.has_component<components::Projectile>();
}

void Projectiles::interProjectileCollision(const events::EntityCollision& collision) {
}

void Projectiles::projectileHit(entityx::Entity projectile, entityx::Entity other) {
  projectile.destroy();
}

}
}
}
