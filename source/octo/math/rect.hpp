#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

#include <cmath>

namespace octo {
namespace math {
namespace rect {

/*! \brief Constructs a rectangle from the position of its center and its size.
 *  \param center the rectangles center position
 *  \param size the size of the rectangle
 *  \returns a rectangle of the desired type.
 */
template <typename T>
sf::Rect<T> fromCenterSize(const sf::Vector2<T>& center, const sf::Vector2<T>& size) {
  return sf::Rect<T>(center - size / static_cast<T>(2), size);
}

/*! \brief Rounds a rectangles corners to integral coordinates outwards.
 *  \param rect a floating point rectangle
 *  \returns the smallest rectangle with integral coordinates that fully contains \p rect.
 */
template <typename T, typename U>
sf::Rect<T> integralOutwards(const sf::Rect<U>& rect) {
  sf::Vector2<T> topLeft = {static_cast<T>(std::floor(rect.left)), static_cast<T>(std::floor(rect.top))};
  sf::Vector2<T> bottomRight = {static_cast<T>(std::ceil(rect.left + rect.width)),
                                static_cast<T>(std::ceil(rect.top + rect.height))};
  return {topLeft, bottomRight - topLeft};
}

/*! \brief Casts the coordinates of the rectangles corners and constructs a new rectangle from these.
 *  \param rect the rect to be cast into a different type.
 *  \returns the casted rectangle.
 */
template<typename T, typename U>
sf::Rect<T> rect_cast(const sf::Rect<U>& rect) {
  sf::Vector2<T> topLeft = {static_cast<T>(rect.left), static_cast<T>(rect.top)};
  sf::Vector2<T> bottomRight = {static_cast<T>(rect.left + rect.width),
                                static_cast<T>(rect.top + rect.height)};
  return {topLeft, bottomRight - topLeft};
}

}
}
}
