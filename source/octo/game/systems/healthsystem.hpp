#pragma once

#include "../components.hpp"
#include "../events/damage.hpp"

#include <fmtlog/fmtlog.hpp>
#include <entityx/entityx.h>

namespace octo {
namespace game {
namespace systems {

/*!
 *  \remark Damn socialists!
 */
struct HealthSystem : public entityx::System<HealthSystem>, public entityx::Receiver<HealthSystem> {

  void configure(entityx::EventManager& events) override;

  void update(entityx::EntityManager& es, entityx::EventManager& events,
              entityx::TimeDelta dt) override;

  void receive(const events::Damage& event);
private:
  fmtlog::Log log = fmtlog::For<HealthSystem>();
};
}
}
}
