#pragma once

#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

namespace octo {
namespace game {
namespace components {

/*! \brief Component defining a planet, consisting of its terrain mask and texture.
 *  \todo Remove terrain mask and add planet-specific properties here, since the mask is now
 *  uniformly stored for all collidable entities in the CollisionMask component.
 */
struct Planet {
  sf::Texture backgroundTexture;
  sf::Texture foregroundTexture;
};

}
}
}
