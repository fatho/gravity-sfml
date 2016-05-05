#include "world.hpp"

#include "components.hpp"
#include "systems/movement.hpp"

using namespace octo::game;

World::World() {
  // configure entity component system
  systems().add<systems::Movement>();
}

void World::addPlanet( sf::Vector2f position, int radius ) {
  entityx::Entity planet = entities().create();

  // add immovable planet
  planet.assign<components::Position>(position);

  // create circular planet texture
  auto planetComp = planet.assign<components::Planet>();
  planetComp->terrain.create(2 * radius, 2 * radius, sf::Color::Transparent);
  for(int y = 0; y < 2 * radius; ++y) {
    for(int x = 0; x < 2 * radius; ++x) {
      int dx = x - radius;
      int dy = y - radius;
      if(dx * dx + dy * dy <= radius * radius) {
        planetComp->terrain.setPixel(x, y, sf::Color::White);
      }
    }
  }
  planetComp->updateTexture();
}

void World::update(sf::Time elapsed) {
  systems().update_all(elapsed.asSeconds());
}

