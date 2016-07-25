#include "world.hpp"

#include "components.hpp"
#include "systems.hpp"
#include "collision/mask.hpp"

using namespace octo::game;

World::World() {
  // configure entity component system (order is important)
  systems.add<systems::Attraction>();
  systems.add<systems::Collision>(*this);
  systems.add<systems::Projectiles>();
  systems.add<systems::Explosions>();
  systems.add<systems::Physics>(*this);
  systems.add<systems::BoundaryEnforcer>(0);
  systems.add<systems::Debug>();
  systems.configure();

  setClipRadius(1000); // depends on m_boundaryEnforcer
}

entityx::Entity World::addPlanet(sf::Vector2f position, int radius, float mass) {
  entityx::Entity planet = entities.create();

  planet.assign<components::Spatial>(position);

  planet.assign<components::Attractor>(
      mass * m_gravitationalConstant, components::Attractor::PlanetBit, radius);

  planet.assign<components::Planet>();
  // TODO: load planet textures

  planet.assign<components::CollisionMask>(collision::circle(radius, collision::Pixel::SolidDestructible));
  return planet;
}

entityx::Entity World::spawnDebugBullet(sf::Vector2f position, sf::Vector2f velocity) {
  entityx::Entity bullet = entities.create();

  bullet.assign<components::Spatial>(position);
  auto body = bullet.assign<components::DynamicBody>();
  body->setMass(1);
  body->setInertia(100);
  body->setVelocity(velocity);
  bullet.assign<components::Attractable>(1, components::Attractable::PlanetBit);
  bullet.assign<components::CollisionMask>(collision::circle(4, collision::Pixel::SolidIndestructible));
  //bullet.assign<components::CollisionMask>(collision::rectangle(8, 20, collision::Pixel::SolidIndestructible));
  bullet.assign_from_copy(components::Projectile { 50 });
  return bullet;
}

void World::update(float timeStep) {
  systems.update_all(timeStep);
  m_updateCount += 1;
}

float World::clipRadius() const {
  return m_clipRadius;
}

void World::setClipRadius(float radius) {
  m_clipRadius = radius;
  systems.system<systems::BoundaryEnforcer>()->setBoundaryRadius(radius);
}

void World::interpolateState(float alpha) {
  using namespace components;

  entities.each<Spatial>([alpha](entityx::Entity, Spatial& spatial) {
    spatial.interpolate(alpha);
  });
}
