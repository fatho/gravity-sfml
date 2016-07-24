#include "debug.hpp"

namespace octo {
namespace game {
namespace systems {

void Debug::configure(entityx::EventManager& events) {
  events.subscribe<events::ComponentModified<components::CollisionMask>>(*this);
  events.subscribe<entityx::ComponentAddedEvent<components::CollisionMask>>(*this);
  events.subscribe<entityx::EntityCreatedEvent>(*this);
}

void Debug::update(entityx::EntityManager& es, entityx::EventManager& events, entityx::TimeDelta dt) {
}

void Debug::receive(const entityx::EntityCreatedEvent& event) {
  // currently, every entity receives a debug data component
  entityx::Entity(event.entity).assign<components::DebugData>();
}

void Debug::receive(const entityx::ComponentAddedEvent<components::CollisionMask>& event) {
  updateCollisionMask(event.component, entityx::Entity(event.entity).component<components::DebugData>());
}

void Debug::receive(const events::ComponentModified<components::CollisionMask>& event) {
  updateCollisionMask(event.component, entityx::Entity(event.entity).component<components::DebugData>());
}

void Debug::updateCollisionMask(entityx::ComponentHandle<components::CollisionMask> collision,
                         entityx::ComponentHandle<components::DebugData> debugData) {
  // make sure the component is still valid by the time the event arrives
  if(collision.valid() && debugData.valid()) {
    auto debugMaskConverter = [](game::collision::Pixel pix) {
      return pix == game::collision::Pixel::NoCollision ? sf::Color::Transparent : sf::Color::White;
    };

    sf::Image img { collision->mask.toImage(debugMaskConverter) };
    debugData->collisionMaskTexture.loadFromImage(img);
  }
}

}
}
}
