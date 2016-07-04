#include "spatial.hpp"

using namespace octo::game::components;

Spatial::Spatial() {
}

Spatial::Spatial(sf::Vector2f pos)
  : position(pos) {}
