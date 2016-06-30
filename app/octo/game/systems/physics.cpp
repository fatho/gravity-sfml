#include "physics.hpp"

using namespace octo::game::systems;

void Physics::update(entityx::EntityManager& es, entityx::EventManager&, entityx::TimeDelta dt) {
  using namespace octo::game::components;
  float dtf = static_cast<float>(dt);
  es.each<Position, DynamicBody>([dtf](entityx::Entity, Position& position, DynamicBody& body) {
      // semi-implicit euler
      body.momentum += body.force * dtf;
      position.position += body.momentum * dtf;
      body.force = sf::Vector2f();
    });
  // TODO: collision detection & response
}
