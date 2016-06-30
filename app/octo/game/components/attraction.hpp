#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Config.hpp>

namespace octo {
namespace game {
namespace components {

constexpr sf::Uint64 PLANET_ATTRACTION = 1;

struct Attractor {
  /// The amount of the attraction force. Negative values mean repulsion.
  float intensity;
  /// Bit-mask used for selectively applying forces to only some entities.
  sf::Uint64 attractionMask;

  Attractor();
  Attractor(float intensity, sf::Uint64 attractionMask);
};

struct Attractable {
  /// Factor to scale the attractive force with.
  float forceFactor;
  /// Bit-mask used for selectively applying forces from only some attractors.
  sf::Uint64 attractionMask;

  Attractable();
  Attractable(float forceFactor, sf::Uint64 attractionMask);
};

}
}
}
