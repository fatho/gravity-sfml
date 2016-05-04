#include "planet.hpp"

using namespace octo::game::components;

void Planet::updateTexture() {
  if(this->terrainTexture.getSize() != this->terrain.getSize()) {
    this->terrainTexture.loadFromImage(this->terrain);
  } else {
    this->terrainTexture.update(this->terrain);
  }
}
