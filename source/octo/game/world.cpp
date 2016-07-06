#include "world.hpp"

#include "components.hpp"
#include "systems/attractionsystem.hpp"
#include "systems/boundaryenforcer.hpp"
#include "systems/physics.hpp"

#include <Box2D/Box2D.h>

#include <stdexcept>

using namespace octo::game;

World::World() {
  m_gravitationalConstant = 100.f;
  // configure entity component system
  systems().add<systems::Attraction>();
  m_physics = systems().add<systems::Physics>();
  m_boundaryEnforcer = systems().add<systems::BoundaryEnforcer>(0);
  setClipRadius(1000); // depends on m_boundaryEnforcer
}

void World::addPlanet(sf::Vector2f position, int radius, float mass) {
  entityx::Entity planet = entities().create();

  // add immovable planet
  planet.assign<components::Spatial>(position);

  planet.assign<components::Attractor>(
      mass * m_gravitationalConstant, components::Attractor::PlanetBit, radius);

  // create circular planet texture
  auto planetComp = planet.assign<components::Planet>();
  planetComp->terrain.create(2 * radius, 2 * radius, sf::Color::Transparent);
  for (int y = 0; y < 2 * radius; ++y) {
    for (int x = 0; x < 2 * radius; ++x) {
      int dx = x - radius;
      int dy = y - radius;
      if (dx * dx + dy * dy <= radius * radius) {
        planetComp->terrain.setPixel(x, y, sf::Color::White);
      }
    }
  }
  planetComp->updateTexture();
}

void World::spawnDebugBullet(sf::Vector2f position, sf::Vector2f velocity) {
  entityx::Entity bullet = entities().create();

  bullet.assign<components::Spatial>(position);
  b2BodyDef bulletDef;
  bulletDef.type = b2_dynamicBody;
  bulletDef.position = {position.x, position.y};
  bulletDef.linearVelocity = {velocity.x, velocity.y};
  bulletDef.linearDamping = 0;
  bulletDef.angularDamping = 0;

  b2CircleShape circ;
  circ.m_p.Set(0, 0);
  circ.m_radius = 1;

  b2Body* body = m_physics->engine().CreateBody(&bulletDef);
  body->CreateFixture(&circ, 1);

  auto bodyComponent = bullet.assign<components::DynamicBody>(body);
  // Assumption: all b2Body's are stored in a component.
  // If the respective entity is destroyed, so is the body.
  // Therefore, the assigned pointer should be valid for the whole lifetime of the body.
  body->SetUserData(bodyComponent.get());

  bullet.assign<components::Attractable>(body->GetMass(), components::Attractable::PlanetBit);
}

void World::update(float timeStep) {
  systems().update_all(timeStep);
}

void World::interpolateState(float alpha) {
  using namespace components;

  m_es.entities.each<Spatial>([alpha](entityx::Entity, Spatial& spatial) {
    spatial.interpolate(alpha);
  });
}

entityx::Entity World::entityOfBody(b2Body& body) {
  void* ptr = body.GetUserData();
  if(ptr) {
    return reinterpret_cast<components::DynamicBody*>(ptr)->entity;
  } else {
    throw std::logic_error("world invariant violated: b2Body not associated with entity");
  }
}

World::~World() {
  m_es.entities.reset();
}
