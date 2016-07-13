#pragma once

#include "../components.hpp"
#include "../events/componentmodified.hpp"

#include <entityx/entityx.h>

namespace octo {
namespace game {
namespace systems {

struct Debug : public entityx::System<Debug>, public entityx::Receiver<Debug> {

  void configure(entityx::EventManager& events) override;

  void update(entityx::EntityManager& es, entityx::EventManager& events,
              entityx::TimeDelta dt) override;

  void receive(const entityx::EntityCreatedEvent& event);

  void receive(const events::ComponentModified<components::Collision>& event);

  void receive(const entityx::ComponentAddedEvent<components::Collision>& event);

private:
  void updateCollisionMask(entityx::ComponentHandle<components::Collision> collision,
                           entityx::ComponentHandle<components::DebugData> debugData);
};
}
}
}
