#pragma once

#include "../components/spatial.hpp"
#include "../components/dynamicbody.hpp"
#include "../components/material.hpp"
#include "../events/entitycollision.hpp"
#include "../world.hpp"

#include <fmtlog/fmtlog.hpp>

#include <entityx/entityx.h>

namespace octo {
namespace game {
namespace systems {

struct Collision : public entityx::System<Collision> {
  Collision(World& world);

  void update(entityx::EntityManager& es, entityx::EventManager& events, entityx::TimeDelta dt) override;

private:

  void bounce(events::EntityCollision& collisionData,
              std::array<entityx::ComponentHandle<components::Material>, 2>& materials);

private:
  fmtlog::Log log = fmtlog::For<Collision>();
  World& m_world;
  int m_normalAccuracy = 3;
};

}
}
}
