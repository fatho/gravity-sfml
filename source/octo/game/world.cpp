#include "world.hpp"

#include "components.hpp"
#include "systems/attractionsystem.hpp"
#include "systems/boundaryenforcer.hpp"
#include "systems/physics.hpp"

using namespace octo::game;

World::World() {
  m_gravitationalConstant = 100.f;
  // configure entity component system
  systems().add<systems::Attraction>();
  systems().add<systems::Physics>();
  m_boundaryEnforcer = systems().add<systems::BoundaryEnforcer>(0);
  setClipRadius(1000); // depends on m_boundaryEnforcer
}

void World::addPlanet(sf::Vector2f position, int radius, float mass) {
  entityx::Entity planet = entities().create();

  // add immovable planet
  planet.assign<components::Position>(position);

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

  bullet.assign<components::Position>(position);
  auto body = bullet.assign<components::DynamicBody>();
  body->setMass(1);
  body->setVelocity(velocity);
  bullet.assign<components::Attractable>(1, components::Attractable::PlanetBit);
}

void World::update(sf::Time elapsed) {
  systems().update_all(elapsed.asSeconds());
}
