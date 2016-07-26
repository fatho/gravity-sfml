#include "projectiles.hpp"

#include "../components.hpp"
#include "../events/explode.hpp"
#include "../events/damage.hpp"

#include <octo/math/vector.hpp>

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
      // apply physical damage proportional to kinetic energy to target of impact
      auto body = hit.entities[0].component<components::DynamicBody>();
      float kineticEnergy = 0.5f * body->mass * math::vector::lengthSquared(body->velocity());
      // FIXME: conversion [energy] -> [damage] is missing
      events.emit(events::Damage { hit.entities[1], 0.01f * kineticEnergy });

      triggerProjectile(events, hit.entities[0]);
    }
  }
  m_projectileHits.clear();

  for (auto& coll : m_projectileCollisions) {
    for (auto& e : coll.entities) {
      if (e.valid()) {
        triggerProjectile(events, e);
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
  if (spatial.valid() && projectile.valid()) {
    log.debug("triggering projectile %s", projectileEntity);
    // TODO replace with scriptable effects
    events.emit<events::Explode>(spatial->current().position, projectile->explosionRadius,
                                 projectile->explosionRadius * 1.5, 100, 1);
    projectile->bounceCounter += 1;
    if(projectile->bounceCounter > 3) {
      projectileEntity.destroy();
    }
  } else {
    log.error("non-projectile %s cannot be triggered", projectileEntity);
  }
}
}
}
}
