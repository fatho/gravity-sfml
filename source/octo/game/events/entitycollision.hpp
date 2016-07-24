#pragma once

#include <entityx/entityx.h>
#include <SFML/System/Vector2.hpp>

#include <array>

namespace octo {
namespace game {
namespace events {

struct EntityCollision {
  std::array<entityx::Entity, 2> entity;
  sf::Vector2f contactPoint;

  /*! \brief Swaps the order of the entities participating in the collision.
   */
  void swapOrder();
};

}
}
}
