#pragma once

#include <SFML/System/Vector2.hpp>
#include <cmath>

namespace octo {
namespace math {
namespace vector {

/*! \brief Computes the dot product of two vectors.
 *  \param va the first vector.
 *  \param vb the second vector.
 *  \returns the dot product of \p va and \p vb.
 */
template<typename T>
inline T dot(const sf::Vector2<T>& va, const sf::Vector2<T>& vb) {
  return va.x * vb.x + va.y * vb.y;
}

/*! \brief Computes the square of the length of the vector.
 *
 *  Note that this function is faster than \ref length, since it only
 *  consists of two multiplications and one addition.
 *
 *  \param vec the vector whose squared length should be computed.
 *  \returns the square of the vector's length.
 */
template<typename T>
inline T lengthSquared(const sf::Vector2<T>& vec) {
  return dot(vec, vec);
}

/*! \brief Computes the length of a vector.
 *
 *  It is computed by taking the square root of \ref lengthSquared.
 *
 *  \param vec the vector whose length should be computed
 *  \returns the length of the vector.
 */
template<typename T>
inline T length(const sf::Vector2<T>& vec) {
  return static_cast<T>(sqrt(lengthSquared(vec)));
}

/*! \brief Normalizes a vector.
 *
 *  \note This function does not check the length of the vector.
 *  If it is called on vectors of (almost) zero length,
 *  the resulting vector might contain \c NaN or \c infinite values.
 *
 *  \param vec the vector to be normalized.
 *  \returns a normalized vector.
 */
template<typename T>
inline sf::Vector2<T> normalized(const sf::Vector2<T>& vec) {
  return vec / length(vec);
}

/*! \brief Computes the cross product of two 2D vectors.
 *
 *  The two vectors are embedded into 3D space by giving them a zero Z coordinate.
 *  From the resulting cross product, only Z is returned, X and Y are always zero.
 *
 *  \param v first vector.
 *  \param w second vector.
 *  \return the Z component of the cross product of the 3D extension of \p v and \p w.
 */
template<typename T>
inline T cross2d(const sf::Vector2<T>& v, const sf::Vector2<T>& w) {
  return v.x * w.y - v.y * w.x;
}

/*! \brief Computes the cross product of a scalar and a 2D vector.
 *
 *  The vectors is embedded into 3D space by giving it a zero Z coordinate, the
 *  scalar is embedded by giving it zero X and Y coordinates.
 *  From the resulting cross product, components X and Y are returned.
 *  The resulting Z coordinate is always zero.
 *
 *  \param s the scalar, interpreted as 3D vector (0, 0, \p s).
 *  \param v the 2D vector, interpreted as 3D vector (\p v.x, \p v.y, 0).
 *  \return the Z component of the cross product of the 3D extension of \p s and \p v.
 */
template<typename T>
inline sf::Vector2<T> cross2d(T s, const sf::Vector2<T>& v) {
  return { - s * v.y, s * v.x };
}

/*! \brief Computes the cross product of a scalar and a 2D vector.
 *
 *  The vectors is embedded into 3D space by giving it a zero Z coordinate, the
 *  scalar is embedded by giving it zero X and Y coordinates.
 *  From the resulting cross product, components X and Y are returned.
 *  The resulting Z coordinate is always zero.
 *
 *  \param v the 2D vector, interpreted as 3D vector (\p v.x, \p v.y, 0).
 *  \param s the scalar, interpreted as 3D vector (0, 0, \p s).
 *  \return the Z component of the cross product of the 3D extension of \p s and \p v.
 */
template<typename T>
inline sf::Vector2<T> cross2d(const sf::Vector2<T>& v, T s) {
  return { s * v.y, - s * v.x };
}

/*! \brief Casts both components of a vector.
 *  \tparam T destination type
 *  \tparam U source type
 *  \param v the vector to be converted.
 *  \returns a vector with \p v's components cast to type \p T.
 */
template<typename T, typename U>
inline sf::Vector2<T> vector_cast(const sf::Vector2<U>& v) {
  return { static_cast<T>(v.x), static_cast<T>(v.y) };
}

/*! \brief Maps over both components of a vector.
 *  \tparam T the source type
 *  \tparam MapFn the type of the mapping function
 *  \param v the vector to be mapped.
 *  \param fn the mapping function.
 *  \returns the mapped vector.
 */
template<typename T, typename MapFn>
inline auto map(const sf::Vector2<T>& v, MapFn fn) -> sf::Vector2<decltype(fn(v.x))> {
  return { fn(v.x), fn(v.y) };
}

inline sf::Vector2f rotate(float radians, const sf::Vector2f& v) {
  float c = std::cos(radians);
  float s = std::sin(radians);
  return { c * v.x - s * v.y, s * v.x + c * v.y };
}

template<typename T>
bool isFinite(const sf::Vector2<T>& v) {
  return std::isfinite(v.x) && std::isfinite(v.y);
}

inline sf::Vector2f unitAngleTrig(float angleRad) {
  return sf::Vector2f(std::cos(angleRad), std::sin(angleRad));
}

}
}
}
