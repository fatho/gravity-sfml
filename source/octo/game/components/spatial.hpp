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

}

/*! \brief Describes the location of an entity in space.
 */
struct Spatial {
  /*! \brief Constructs a spatial component from a position.
   *
   *  The rotation is initialized to \c 0.
   *  \param pos the initial position.
   */
  Spatial(sf::Vector2f pos);

  /// The position of the origin \c (0,0) with no rotation.
  Spatial();


  /*! \brief Returns the entity's rotation in radians in counter-clockwise order.
   *
   *  Note that the angles direction is reversed compared to the \ref rotationDegrees member.
   *  This function returns an angle that can directly be used in trigonometric functions,
   *  expecting counter-clockwise radians.
   *
   *  \return The entity's rotation in radians in counter-clockwise order.
   */
  float rotationRadians() const {
    return internal::RadView<const float>(rotationDegrees);
  }


  /*! \brief Returns a modifiable view presenting the entity's rotation in radians in counter-clockwise order.
   *
   *  Note that the angles direction is reversed compared to the \ref rotationDegrees member.
   *  This function returns an angle that can directly be used in trigonometric functions,
   *  expecting counter-clockwise radians.
   *
   *  \return The a view of the entity's rotation in radians in counter-clockwise order.
   *  \warning The returned reference is only valid as long a this instance is still alive.
   */
  internal::RadView<float> rotationRadians() {
    return internal::RadView<float>(rotationDegrees);
  }

  /*! \brief Sets the entity's rotation from counter-clockwise angle in radians.
   *
   *  It is internally converted to degrees in clockwise order and stored
   *  in the \ref rotationDegrees member.
   *  \param radCCW the new rotation angle in radians in counter-clockwise order.
   */
  void setRotationRadians(float radCCW) {
    rotationDegrees = - radCCW * 180.f / boost::math::constants::pi<float>();
  }

  /// the entity's position
  sf::Vector2f position;

  /*! \brief The entity's rotation in degrees in clockwise order.
   *
   *  The rotation is directly compatible with SFML.
   */
  float rotationDegrees;
};

}
}
}
