#pragma once

#include "../events/explode.hpp"
#include <fmtlog/fmtlog.hpp>
#include <entityx/entityx.h>

#include <vector>

namespace octo {
namespace game {
namespace systems {

struct Explosions : public entityx::System<Explosions>, public entityx::Receiver<Explosions> {
  Explosions();

  void configure(entityx::EventManager& events) override;

  void update(entityx::EntityManager& es, entityx::EventManager& events, entityx::TimeDelta dt) override;

  void receive(const events::Explode& event);

private:
  fmtlog::Log log = fmtlog::For<Explosions>();

  std::vector<events::Explode> m_explosions;
};

}
}
}
