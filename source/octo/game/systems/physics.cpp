#include "physics.hpp"

using namespace octo::game::systems;

void Physics::update(entityx::EntityManager& es, entityx::EventManager&, entityx::TimeDelta dt) {
  using namespace octo::game::components;
  float dtf = static_cast<float>(dt);
  es.each<Spatial, DynamicBody>([dtf](entityx::Entity, Spatial& spatial, DynamicBody& body) {
      // semi-implicit euler
      body.momentum += body.force * dtf;
      spatial.position += body.velocity() * dtf;
      body.force = sf::Vector2f();
    });
  // TODO: collision detection & response
}
