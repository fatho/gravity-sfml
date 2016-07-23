#pragma once

#include "../events/entitycollision.hpp"
#include <fmtlog/fmtlog.hpp>
#include <entityx/entityx.h>

namespace octo {
namespace game {
namespace systems {

struct Projectiles : public entityx::System<Projectiles>, public entityx::Receiver<Projectiles> {
  Projectiles();

  void configure(entityx::EventManager& events) override;

  void update(entityx::EntityManager& es, entityx::EventManager& events, entityx::TimeDelta dt) override;

  void receive(const events::EntityCollision& event);

private:

  static bool isProjectile(entityx::Entity entity);

  /*! \brief Handles collisions between two projectiles.
   *
   *  \remark In this case, all of the relevant logic is contained in this system.
   */
  void interProjectileCollision(const events::EntityCollision& collision);

  /*! \brief Handles collisions between a projectile and something else.
   */
  void projectileHit(entityx::Entity projectile, entityx::Entity other);

private:
  fmtlog::Log log = fmtlog::For<Projectiles>();
};

}
}
}
