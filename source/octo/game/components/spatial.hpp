#pragma once

#include <cmath>
#include <boost/math/constants/constants.hpp>
#include <SFML/System/Vector2.hpp>

namespace octo {
namespace game {
namespace components {

namespace internal {

template<typename T>
class RadView {
public:
  RadView(T& degrees)
    : m_degrees(degrees) {}

  RadView& operator=(float radians) {
    m_degrees = ccwRadToCwDeg(radians);
    return *this;
  }

  RadView& operator+=(float radians) {
    m_degrees += ccwRadToCwDeg(radians);
    return *this;
  }

  operator float() const {
    return - m_degrees / static_cast<T>(180.0) * boost::math::constants::pi<T>();
  }

private:
  float ccwRadToCwDeg(float ccwrad) {
    return - ccwrad * static_cast<T>(180.0) / boost::math::constants::pi<T>();
  }

  T& m_degrees;
};

template<typename T>
class Interpolated {
public:
  Interpolated() {}
  Interpolated(const T& value) {
    setAll(value);
  }

  T& current() {
    return m_current;
  }

  const T& current() const {
    return m_current;
  }

  T& previous() {
    return m_previous;
  }

  const T& previous() const {
    return m_previous;
  }

  T& interpolated() {
    return m_interpolated;
  }

  const T& interpolated() const {
    return m_interpolated;
  }

  void interpolate(float alpha) {
    m_interpolated = m_previous * (1 - alpha) + m_current * alpha;
  }

  void setAll(const T& value) {
    m_current = m_previous = m_interpolated = value;
  }
private:
  T m_previous;
  T m_current;
  T m_interpolated;
};

}

/*! \brief Describes the location of an entity in space.
 */
struct SpatialSnapshot {
public:
  /// the entity's position
  sf::Vector2f position = sf::Vector2f();

  /*! \brief The entity's rotation in degrees in clockwise order.
   *
   *  The rotation is directly compatible with SFML.
   */
  float rotationDegrees = 0;

public:
  /*! \brief Constructs a spatial component from a position and rotation.
   *
   *  \param pos the initial position.
   *  \param rotation the initial rotation in degrees in clockwise order.
   */
  SpatialSnapshot(sf::Vector2f pos = sf::Vector2f(), float rotation = 0);


  /*! \brief Returns the entity's rotation in radians in counter-clockwise order.
   *
   *  Note that the angles direction is reversed compared to the \ref rotationDegrees member.
   *  This function returns an angle that can directly be used in trigonometric functions,
   *  expecting counter-clockwise radians.
   *
   *  \return The entity's rotation in radians in counter-clockwise order.
   */
  float rotationRadians() const;

  /*! \brief Returns a modifiable view presenting the entity's rotation in radians in counter-clockwise order.
   *
   *  Note that the angles direction is reversed compared to the \ref rotationDegrees member.
   *  This function returns an angle that can directly be used in trigonometric functions,
   *  expecting counter-clockwise radians.
   *
   *  \return The a view of the entity's rotation in radians in counter-clockwise order.
   *  \warning The returned reference is only valid as long a this instance is still alive.
   */
  internal::RadView<float> rotationRadians();

  /*! \brief Sets the entity's rotation from counter-clockwise angle in radians.
   *
   *  It is internally converted to degrees in clockwise order and stored
   *  in the \ref rotationDegrees member.
   *  \param radCCW the new rotation angle in radians in counter-clockwise order.
   */
  void setRotationRadians(float radCCW);
};

struct Spatial : public internal::Interpolated<SpatialSnapshot> {
  Spatial(sf::Vector2f pos = sf::Vector2f(), float rotation = 0);
};

// interpolation of SpatialSnapshot values

/*! \brief Scales a SpatialSnapshot value by a factor \c alpha.
 *  \param spatial the SpatialSnapshot value
 *  \param alpha the scaling factor
 *  \return a new SpatialSnapshot value computed from \c spatial by multiplying every field with \c alpha.
 */
SpatialSnapshot operator*(const SpatialSnapshot& spatial, float alpha);

/*! \brief Adds two SpatialSnapshot values component-wise.
 *  \param a the first value.
 *  \param b the second value.
 *  \return the component-wise sum of \p a and \p b.
 */
SpatialSnapshot operator+(const SpatialSnapshot& a, const SpatialSnapshot& b);

/*! \brief Performs a linear interpolation between two spatial values.
 *
 *  It computes \code a * (1 - alpha) + b * alpha \endcode.
 *
 *  \param a the starting value.
 *  \param b the final value.
 *  \param alpha the blend factor where \c 0 represents \p a and \c 1 represents \p b.
 *  \returns a linear interpolation between \p a and \p b.
 */
SpatialSnapshot lerp(const SpatialSnapshot& a, const SpatialSnapshot& b, float alpha);

}
}
}
