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

/*! \brief Checks whether a pixel is solid.
 *
 *  Only solid pixels participate in the (pixel-perfect) collision detection.
 */
inline bool isSolid(Pixel pix) {
  return pix != Pixel::NoCollision;
}

/*! \brief Checks whether a pixel can be destroyed.
 *
 *  Only destructible parts of a collision mask are affected by explosions.
 */
inline bool isDestructible(Pixel pix) {
  return pix == Pixel::SolidDestructible;
}

/*! \brief Creates a circular collision mask.
 *
 *  The circle is filled with \p fill, all pixels outside of the circle are set to Pixel::NoCollision.
  * The  collision mask will be a square with edges of length `2 * radius + 1`.
 *
 *  \param radius The radius of the circle.
 *  \param fill The fill value of the circle.
 */
Mask circle(size_t radius, Pixel fill);

/*! \brief Creates a rectangular collision mask.
 *
 *  The rectangle is filled with \p fill and it fills the collision mask completely.
 *
 *  \param width The width of the collision mask.
 *  \param height The height of the collision mask.
 *  \param fill The fill value of the rectangle.
 */
Mask rectangle(size_t width, size_t height, Pixel fill);

/*! \brief Creates an elliptical collision mask.
 *
 *  The ellipse is filled with \p fill, all pixels outside of the circle are set to Pixel::NoCollision.
 *
 *  \param width The width of the collision mask.
 *  \param height The height of the collision mask.
 *  \param fill The fill value of the ellipse.
 */
Mask ellipse(size_t width, size_t height, Pixel fill);

}
}
}
