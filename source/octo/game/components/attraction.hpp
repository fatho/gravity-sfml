#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Config.hpp>

namespace octo {
namespace game {
namespace components {

/*! \brief Parameters describing an attractive force.
 */
struct AttractionParameters {
  /*! The amount of the attraction force. Negative values mean repulsion.
   *
   *  The final force is computed from the product of the intensities off
   *  the \ref Attractor and the \ref Attractable, and is subject to a quadratic
   *  falloff w.r.t. distance.
   */
  float intensity = 0;
  /*! \brief Bit-mask used for selectively applying forces to only some entities.
   *
   *  The reserved bits are
   *    - \ref AttractionParameters::PlanetBit.
   */
  sf::Uint64 attractionMask = 0;

  /// Flag for gravitational attraction exerted by planets.
  static constexpr sf::Uint64 PlanetBit = 1;

  /// Initializes an inactive attractor with no force and an empty mask.
  AttractionParameters() = default;
  /*! \brief Initializes the attraction parameters.
   *  \param intensity \ref AttractionParameters::intensity
   *  \param attractionMask \ref AttractionParameters::attractionMask
   */
  AttractionParameters(float intensity, sf::Uint64 attractionMask);
};

/*! \brief Attached to an entity that exerts an attractive force on other entities.
 */
struct Attractor : public AttractionParameters {
  /*! \brief The radius of the attractor.
   *
   *  Within this radius, the attraction force decreases towards the center, where it becomes 0.
   */
  float radius = 0;

  using AttractionParameters::AttractionParameters;

  /*! \brief Initializes the the attraction parameters and the radius of the attractor.
   *
   *  \param intensity attraction intensity
   *  \param attractionMask attraction mask for filtering which entities interact with each other
   *  \param radius the radius of the attractor.
   *  Inside this radius, the force will decrease towards the objects center.
   */
  Attractor(float intensity, sf::Uint64 attractionMask, float radius);
};

/*! \brief Marks an entity that is attracted by \ref Attractor "Attractor"s.
 */
struct Attractable : public AttractionParameters {
  using AttractionParameters::AttractionParameters;
};

}
}
}
