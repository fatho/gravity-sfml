#include "physics.hpp"

using namespace octo::game::systems;

Physics::Physics()
  : m_b2world({0,0})
{
}

void Physics::update(entityx::EntityManager& es, entityx::EventManager&, entityx::TimeDelta dt) {
  using namespace components;
  float timeStep = static_cast<float>(dt);
  // simulate physics
  m_b2world.Step(timeStep, 8, 3);
  // update current and previous position
  es.each<Spatial, DynamicBody>([](entityx::Entity, Spatial& spatial, DynamicBody& body) {
      spatial.previous() = spatial.current();
      b2Vec2 pos = body->GetPosition();
      spatial.current().position = {pos.x, pos.y};
      spatial.current().rotationRadians() = body->GetAngle();
    });
}

