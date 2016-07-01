#include "attraction.hpp"

using namespace octo::game::components;

constexpr sf::Uint64 AttractionParameters::PlanetBit;

AttractionParameters::AttractionParameters(float intensity, sf::Uint64 attractionMask)
    : intensity(intensity), attractionMask(attractionMask) {}


Attractor::Attractor(float intensity, sf::Uint64 attractionMask, float radius)
  : AttractionParameters(intensity, attractionMask), radius(radius) {
}
