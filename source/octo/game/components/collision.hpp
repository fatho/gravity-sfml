#pragma once

#include <octo/util/pixelarray.hpp>

#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

namespace octo {
namespace game {
namespace components {

/*! \brief Component defining a planet, consisting of its terrain mask and texture.
 */
struct Collision {
  /*! \brief the 8-bit collision mask representing the shape of the entity.
   *
   *  All pixels with a value not equal to zero are considered to be part of the object.
   *  Bytes are used instead of just one bit per pixel to allow specific behavior depending
   *  on which part of an object collided (e.g. when some parts are bouncier than others).
   */
  util::PixelArray<sf::Uint8> mask;
};

}
}
}
