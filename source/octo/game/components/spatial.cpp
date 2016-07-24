#include "spatial.hpp"

namespace octo {
namespace game {
namespace components {

SpatialSnapshot::SpatialSnapshot(sf::Vector2f pos, float rot)
  : position(pos), rotationDegrees(rot) {}

float SpatialSnapshot::rotationRadians() const {
  return internal::RadView<const float>(rotationDegrees);
}

internal::RadView<float> SpatialSnapshot::rotationRadians() {
  return internal::RadView<float>(rotationDegrees);
}

sf::Transform SpatialSnapshot::localToGlobal() const {
  sf::Transform t;
  t.rotate(this->rotationDegrees);     // ... -> local rotated A
  t.translate(this->position);         // ... -> global
  return t;
}

sf::Transform SpatialSnapshot::globalToLocal() const {
  sf::Transform t;
  t.translate(-this->position);         // ... -> local rotated A
  t.rotate(-this->rotationDegrees);     // ... -> local
  return t;
}

Spatial::Spatial(sf::Vector2f pos, float rotation)
  : Interpolated<SpatialSnapshot>(SpatialSnapshot(pos, rotation))
{
}

SpatialSnapshot operator*(const SpatialSnapshot& spatial, float alpha) {
  return SpatialSnapshot {spatial.position * alpha, spatial.rotationDegrees * alpha};
}

SpatialSnapshot operator+(const SpatialSnapshot& a, const SpatialSnapshot& b) {
  return SpatialSnapshot {a.position + b.position, a.rotationDegrees + b.rotationDegrees};
}

SpatialSnapshot lerp(const SpatialSnapshot& a, const SpatialSnapshot& b, float alpha) {
  return a * (1 - alpha) + b * alpha;
}

}
}
}
