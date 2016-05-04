#pragma once

#include <entityx/entityx.h>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

namespace octo {
namespace game {

class World {
public:
  World();

  void addPlanet(sf::Vector2f position, int radius);

  void update(sf::Time elapsed);

  // accessors

  entityx::EntityManager& entities() {
    return m_es.entities;
  }

  entityx::SystemManager& systems() {
    return m_es.systems;
  }

  entityx::EventManager& events() {
    return m_es.events;
  }

private:
  entityx::EntityX m_es;
};

}
}
