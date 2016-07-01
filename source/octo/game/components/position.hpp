#pragma once

#include <SFML/System/Vector2.hpp>

namespace octo {
namespace game {
namespace components {

/*! \brief Assigns a position to an entity.
 */
struct Position {
  /*! \brief Initializes the position.
   *  \param pos the initial position.
   */
  Position(sf::Vector2f pos);

  /// The position of the origin \c (0,0).
  Position();

  /// the associated position
  sf::Vector2f position;
};

}
}
}
