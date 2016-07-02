#pragma once

#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

namespace octo {
namespace game {
namespace components {

/*! \brief Component defining a planet, consisting of its terrain mask and texture.
 */
struct Planet {
  /// the image of the terrain used for physics
  sf::Image terrain;
  /// the replication of the terrain image in VRAM (slow to modify/query)
  sf::Texture terrainTexture;

  /*! \brief Returns the size of the planet.
   *  \returns the size of the planet
   *  \remark This function is currently returning the size of the planets terrain texture.
   */
  sf::Vector2f size() const;

  /*! \brief Updates the texture in VRAM to reflect changes made to the terrain.
   */
  void updateTexture();
};

}
}
}
