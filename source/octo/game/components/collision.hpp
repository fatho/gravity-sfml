#pragma once

#include <octo/game/collision/mask.hpp>

#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

namespace octo {
namespace game {
namespace components {

/*! \brief Component defining a planet, consisting of its terrain mask and texture.
 */
struct Collision {
  /*! \brief Collision mask representing the shape of the entity.
   */
  collision::Mask mask;
  /*! \brief The position of the collision masks center in the entities local coordinate system.
   */
  sf::Vector2f anchor;

  Collision();

  Collision(collision::Mask mask);

  Collision(collision::Mask mask, sf::Vector2f anchor);
};

}
}
}
