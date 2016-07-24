#pragma once

#include "../events/entitycollision.hpp"
#include <fmtlog/fmtlog.hpp>
#include <entityx/entityx.h>

#include <vector>

namespace octo {
namespace game {
namespace systems {

struct Projectiles : public entityx::System<Projectiles>, public entityx::Receiver<Projectiles> {
  Projectiles();

  void configure(entityx::EventManager& events) override;

  void update(entityx::EntityManager& es, entityx::EventManager& events, entityx::TimeDelta dt) override;

  void receive(const events::EntityCollision& event);

private:
  /*! \brief Checks whether an entity is a projectile.
   *
   *  Everything with a component::Projectile component is a projectile.
   *
   *  \returns \c true if and only if \p entity is a projectile.
   */
  static bool isProjectile(entityx::Entity entity);

  /*! \brief Triggers the effects of a projectile.
   *
   *  Typically, this would happen due to the projectile colliding with something,
   *  but ultimately, it depends on the specific type of the projectile.
   *
   *  \param events the event manager
   *  \param projectileEntity the projectile being triggered
   */
  void triggerProjectile(entityx::EventManager& events, entityx::Entity projectileEntity);

private:
  fmtlog::Log log = fmtlog::For<Projectiles>();

  std::vector<events::EntityCollision> m_projectileCollisions;
  std::vector<events::EntityCollision> m_projectileHits;
};

}
}
}
