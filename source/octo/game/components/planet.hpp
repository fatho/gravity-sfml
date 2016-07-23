#pragma once

#include <SFML/Graphics/Texture.hpp>

namespace octo {
namespace game {
namespace components {

/*! \brief Component defining a planet, consisting of its terrain mask and texture.
 *
 *  \todo add atmospheric parameters
 */
struct Planet {
  /*! \brief Texture of the planets background.
   *
   *   It should fill the parts of the planet where terrain is missing,
   *   but that actually belong to the planet.
   */
  sf::Texture backgroundTexture;
  /*! \brief Defines the texture of the planets terrain.
   */
  sf::Texture foregroundTexture;
};

}
}
}
