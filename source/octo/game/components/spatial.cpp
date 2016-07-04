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

void SpatialSnapshot::setRotationRadians(float radCCW) {
  rotationDegrees = - radCCW * 180.f / boost::math::constants::pi<float>();
}

Spatial::Spatial(sf::Vector2f pos, float rotation)
  : Interpolated<SpatialSnapshot>(SpatialSnapshot(pos, rotation))
{ 
}

SpatialSnapshot operator*(const SpatialSnapshot& spatial, float alpha) {
  return {spatial.position * alpha, spatial.rotationDegrees * alpha};
}

SpatialSnapshot operator+(const SpatialSnapshot& a, const SpatialSnapshot& b) {
  return {a.position + b.position, a.rotationDegrees + b.rotationDegrees};
}

SpatialSnapshot lerp(const SpatialSnapshot& a, const SpatialSnapshot& b, float alpha) {
  return a * (1 - alpha) + b * alpha;
}

}
}
}
