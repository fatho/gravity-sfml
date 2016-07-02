#include "planet.hpp"

using namespace octo::game::components;

void Planet::updateTexture() {
  if(this->terrainTexture.getSize() != this->terrain.getSize()) {
    this->terrainTexture.loadFromImage(this->terrain);
  } else {
    this->terrainTexture.update(this->terrain);
  }
}

sf::Vector2f Planet::size() const {
  sf::Vector2u size = terrain.getSize();
  return { static_cast<float>(size.x), static_cast<float>(size.y) };
}
