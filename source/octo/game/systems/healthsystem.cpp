#include "healthsystem.hpp"

#include "../components.hpp"

namespace octo {
namespace game {
namespace systems {

void HealthSystem::configure(entityx::EventManager& events) {
  events.subscribe<events::Damage>(*this);
}

void HealthSystem::update(entityx::EntityManager& es, entityx::EventManager& events,
            entityx::TimeDelta dt) {
}

void HealthSystem::receive(const events::Damage& event) {
  auto target = const_cast<entityx::Entity&>(event.target);
  auto health = target.component<components::Health>();
  if(health) {
    log.debug("applying %.1f damage to [%s]", event.damage);
    if(health->damage(event.damage)) {
      log.debug("entity [%s] killed", target.id());
      target.destroy();
    }
  } else {
    log.debug("applying %.1f damage to entity [%s] without health component", event.damage, event.target.id());
  }
}

}
}
}
