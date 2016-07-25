#pragma once

#include <entityx/entityx.h>
#include <SFML/System/Vector2.hpp>

#include <array>

namespace octo {
namespace game {
namespace events {

struct EntityCollision {
  std::array<entityx::Entity, 2> entities;
  std::array<sf::Vector2f, 2> normals;
  sf::Vector2f contactPoint;

  EntityCollision(const std::array<entityx::Entity, 2>& collisionEntities,
                  const std::array<sf::Vector2f, 2>& collisionNormals,
                  const sf::Vector2f& contactPoint);

  /*! \brief Swaps the order of the entities participating in the collision.
   */
  void swapOrder();
};

}
}
}
