#include "debug.hpp"

namespace octo {
namespace game {
namespace systems {

void Debug::configure(entityx::EventManager& events) {
  events.subscribe<events::ComponentModified<components::Collision>>(*this);
  events.subscribe<entityx::ComponentAddedEvent<components::Collision>>(*this);
  events.subscribe<entityx::EntityCreatedEvent>(*this);
}

void Debug::update(entityx::EntityManager& es, entityx::EventManager& events, entityx::TimeDelta dt) {
}

void Debug::receive(const entityx::EntityCreatedEvent& event) {
  entityx::Entity(event.entity).assign<components::DebugData>();
}

void Debug::receive(const entityx::ComponentAddedEvent<components::Collision>& event) {
  updateCollisionMask(event.component, entityx::Entity(event.entity).component<components::DebugData>());
}

void Debug::receive(const events::ComponentModified<components::Collision>& event) {
  updateCollisionMask(event.component, entityx::Entity(event.entity).component<components::DebugData>());
}

void Debug::updateCollisionMask(entityx::ComponentHandle<components::Collision> collision,
                         entityx::ComponentHandle<components::DebugData> debugData) {
  auto debugMaskConverter = [](game::collision::Pixel pix) {
    return pix == game::collision::Pixel::NoCollision ? sf::Color::Transparent : sf::Color::White;
  };

  sf::Image img { collision->mask.toImage(debugMaskConverter) };
  debugData->collisionMaskTexture.loadFromImage(img);
}

}
}
}
