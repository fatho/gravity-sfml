#pragma once

#include <octo/util/pixelarray.hpp>

namespace octo {
namespace game {
namespace collision {

/*! \brief Type of collision mask pixels.
 *
 *  All values between NoCollision and Solid are available for user-specific uses.
 */
enum class Pixel : sf::Uint8 {
  /// Value for pixels that are not part of the entity
  NoCollision = 0,
  /// Default value for solid pixels without special properties
  Solid = 0xFF
};

/*! \brief An 8-bit collision mask representing the shape of the entity.
 *
 *  All pixels with a value not equal to zero are considered to be part of the object.
 *  Bytes are used instead of just one bit per pixel to allow specific behavior depending
 *  on which part of an object collided (e.g. when some parts are bouncier than others).
 */
using Mask = util::PixelArray<Pixel>;

Mask circle(size_t radius, Pixel fill = Pixel::Solid);

Mask rectangle(size_t width, size_t height, Pixel fill = Pixel::Solid);
}
}
}
