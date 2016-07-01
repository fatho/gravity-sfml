#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Config.hpp>

namespace octo {
namespace game {
namespace components {

constexpr sf::Uint64 PLANET_ATTRACTION = 1;

/*! \brief Parameters describing an attractive force.
 */
struct AttractionParameters {
  /// The amount of the attraction force. Negative values mean repulsion.
  float intensity = 0;
  /// Bit-mask used for selectively applying forces to only some entities.
  sf::Uint64 attractionMask = 0;

  AttractionParameters() = default;
  AttractionParameters(float intensity, sf::Uint64 attractionMask);
};

struct Attractor : public AttractionParameters {
  /*! \brief The radius of the attractor.
   *
   *  Within this radius, the attraction force decreases towards the center, where it becomes 0.
   */
  float radius = 0;

  using AttractionParameters::AttractionParameters;

  Attractor(float intensity, sf::Uint64 attractionMask, float radius);
};

struct Attractable : public AttractionParameters {
  using AttractionParameters::AttractionParameters;
};

}
}
}
