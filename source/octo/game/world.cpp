#include "world.hpp"

#include "components.hpp"
#include "systems/attractionsystem.hpp"
#include "systems/boundaryenforcer.hpp"
#include "systems/physics.hpp"
#include "collision/mask.hpp"

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
  planet.assign<components::Spatial>(position);

  planet.assign<components::Attractor>(
      mass * m_gravitationalConstant, components::Attractor::PlanetBit, radius);

  planet.assign<components::Planet>();
  // TODO: load planet textures

  // generate terrain mask
  planet.assign<components::Collision>(collision::circle(radius, collision::Pixel::Solid));
}

void World::spawnDebugBullet(sf::Vector2f position, sf::Vector2f velocity) {
  entityx::Entity bullet = entities().create();

  bullet.assign<components::Spatial>(position);
  auto body = bullet.assign<components::DynamicBody>();
  body->setMass(1);
  body->setInertia(1);
  body->setVelocity(velocity);
  bullet.assign<components::Attractable>(1, components::Attractable::PlanetBit);
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
