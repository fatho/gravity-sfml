#include "attraction.hpp"

using namespace octo::game::components;

Attractor::Attractor() : intensity(0), attractionMask(0) {}

Attractor::Attractor(float intensity, sf::Uint64 attractionMask)
    : intensity(intensity), attractionMask(attractionMask) {}

Attractable::Attractable() : forceFactor(0), attractionMask(0) {}

Attractable::Attractable(float forceFactor, sf::Uint64 attractionMask)
    : forceFactor(forceFactor), attractionMask(attractionMask) {}
