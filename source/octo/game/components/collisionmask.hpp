#pragma once

#include <octo/game/collision/mask.hpp>
#include <octo/math/vector.hpp>

#include <SFML/Config.hpp>

namespace octo {
namespace game {
namespace components {

/*! \brief Component defining a planet, consisting of its terrain mask and texture.
 */
struct CollisionMask {
  /*! \brief Collision mask representing the shape of the entity.
   */
  collision::Mask mask;
  /*! \brief The position of the collision masks center in the entities local coordinate system.
   */
  sf::Vector2f anchor;
  /*! \brief The collision selector mask, used to prevent certain entities from colliding.
   *
   *  Only if the two overlapping collision masks have at least one common bit set in
   *  their selectors, the entities actually collide.
   */
  sf::Uint64 selector = 0xFFFFFFFFFFFFFFFFUL;

  /*! \brief A sensor collision mask only reports collisions, but does not "bounce".
   */
  bool sensor = false;

  sf::Vector2f size() const {
    return math::vector::vector_cast<float>(mask.size());
  }

  CollisionMask();

  CollisionMask(collision::Mask mask);

  CollisionMask(collision::Mask mask, sf::Vector2f anchor);
};

}
}
}
