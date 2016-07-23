#pragma once

#include <octo/util/interpolation.hpp>

#include <cmath>
#include <boost/math/constants/constants.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Transform.hpp>

namespace octo {
namespace game {
namespace components {

namespace internal {


/*! \brief View class transparently exposing an underlying angle in degrees as radians.
 */
template<typename T>
class RadView {
public:
  /*! \brief Initializes the view.
   *  \param degress a reference to the underlying variable holding the angle in degrees.
   */
  RadView(T& degrees)
    : m_degrees(degrees) {}

  /*! \brief Assigns an angle in radians.
   *
   *  Internally, it converts the angle to degrees and assigns it to the encapsulated reference.

   *  \param radians the angle in radians.
   *  \returns a reference to this view.
   */
  RadView& operator=(float radians) {
    m_degrees = radians * RadToDeg;
    return *this;
  }

  /*! \brief Adds an angle in radians.
   *
   *  Internally, the angle is converted to degrees before being added to the encapsulated reference.
   *  \param radians the angle that is added.
   *  \returns a reference to this view.
   */
  RadView& operator+=(float radians) {
    m_degrees += radians * RadToDeg;
    return *this;
  }

  /*! \brief Converts the value of the underlying reference to radians.
   *  \returns the referenced angle in radians.
   */
  operator float() const {
    return m_degrees / RadToDeg;
  }

private:
  static constexpr float RadToDeg = 180.f / boost::math::constants::pi<float>();

  T& m_degrees;
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


  /*! \brief Returns the entity's rotation in radians in clockwise order.
   *
   *  \return The entity's rotation in radians in clockwise order.
   */
  float rotationRadians() const;

  /*! \brief Returns a modifiable view presenting the entity's rotation in radians in clockwise order.
   *
   *  \return The a view of the entity's rotation in radians in clockwise order.
   *  \warning The returned reference is only valid as long a this instance is still alive.
   */
  internal::RadView<float> rotationRadians();

  sf::Transform localToGlobal() const;

  sf::Transform globalToLocal() const;
};

struct Spatial : public util::Interpolated<SpatialSnapshot> {
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
