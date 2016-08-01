#pragma once

#include "systems/boundaryenforcer.hpp"

#include <entityx/entityx.h>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Transform.hpp>

#include <memory>

namespace octo {
namespace game {

class World : public entityx::EntityX {
public:
  World();

  entityx::Entity addPlanet(sf::Vector2f position, int radius, float mass);

  entityx::Entity spawnDebugBullet(sf::Vector2f position, sf::Vector2f velocity);

  entityx::Entity spawnVessel(sf::Vector2f position, float rotation);

  void update(float timeStep);

  // accessors

  /**
   * @brief The radius defining the outer boundary of the world.
   *
   * Every entity moving past this boundary will be destroyed.
   */
  float clipRadius() const;

  /**
   * @brief Sets the outer boundary of the world.
   * @param radius the new radius of the boundary
   */
  void setClipRadius(float radius);

  /*! \brief Interpolates the world state between the current and last update.
   *
   *  The following entity types are affected:
   *    - the interpolated position of dynamic bodies is stored in
   *      their \ref components::Spatial "Spatial" component.
   *
   *  \param alpha the interpolation function
   */
  void interpolateState(float alpha);

private:
  float m_clipRadius;
  float m_gravitationalConstant = 100.f;
  size_t m_updateCount = 0;
};

}
}
