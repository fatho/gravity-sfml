#pragma once

#include <octo/util/pixelarray.hpp>

#include <array>

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
  /// solid, destructible pixels without special properties
  SolidIndestructible = 0xFE,
  /// solid, destructible pixels without special properties
  SolidDestructible = 0xFF,
};

/*! \brief An 8-bit collision mask representing the shape of the entity.
 *
 *  All pixels with a value not equal to zero are considered to be part of the object.
 *  Bytes are used instead of just one bit per pixel to allow specific behavior depending
 *  on which part of an object collided (e.g. when some parts are bouncier than others).
 */
using Mask = util::PixelArray<Pixel>;

// some accessors to facilitate later changes in representation

inline bool isSolid(Pixel pix) {
  return pix != Pixel::NoCollision;
}

inline bool isDestructible(Pixel pix) {
  return pix == Pixel::SolidDestructible;
}

Mask circle(size_t radius, Pixel fill);

Mask rectangle(size_t width, size_t height, Pixel fill);

Mask ellipse(size_t width, size_t height, Pixel fill);

}
}
}
