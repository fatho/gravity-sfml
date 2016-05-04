#include "movement.hpp"

using namespace octo::game::systems;

void Movement::update(entityx::EntityManager& es, entityx::EventManager&, entityx::TimeDelta dt) {
  using namespace octo::game::components;
  es.each<Position, Movable>([dt](entityx::Entity, Position& position, Movable& movable) {
    position.position += movable.velocity * static_cast<float>(dt);
    movable.velocity += movable.acceleration * static_cast<float>(dt);
  });
}
