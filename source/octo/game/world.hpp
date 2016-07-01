#pragma once

#include "systems/boundaryenforcer.hpp"

#include <entityx/entityx.h>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

#include <memory>

namespace octo {
namespace game {

class World {
public:
  World();

  void addPlanet(sf::Vector2f position, int radius, float mass);

  void spawnDebugBullet(sf::Vector2f position, sf::Vector2f velocity);

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

  /**
   * @brief The radius defining the outer boundary of the world.
   *
   * Every entity moving past this boundary will be destroyed.
   */
  float clipRadius() const {
    return m_clipRadius;
  }

  /**
   * @brief Sets the outer boundary of the world.
   * @param radius the new radius of the boundary
   */
  void setClipRadius(float radius) {
    m_clipRadius = radius;
    m_boundaryEnforcer->setBoundaryRadius(radius);
  }

private:
  entityx::EntityX m_es;
  float m_clipRadius;
  std::shared_ptr<systems::BoundaryEnforcer> m_boundaryEnforcer;
  float m_gravitationalConstant;
};

}
}
