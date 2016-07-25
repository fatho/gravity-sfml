#include "util.hpp"

namespace octo {
namespace game {
namespace collision {

sf::Vector2f computeNormal(const Mask& mask, int accuracy, size_t x, size_t y) {
  sf::Vector2f normalTmp {0,0};
  sf::Rect<int> maskBB { {0,0}, math::vector::vector_cast<int>(mask.size()) };
  for(int dx = -accuracy; dx <= accuracy; ++dx) {
    for(int dy = -accuracy; dy <= accuracy; ++dy) {
      int px = static_cast<int>(x) + dx;
      int py = static_cast<int>(y) + dy;
      if(maskBB.contains(px, py)) {
        if(isSolid(mask.at(px, py))) {
          normalTmp += { static_cast<float>(dx), static_cast<float>(dy) };
        }
      }
    }
  }
  if(normalTmp.x != 0 || normalTmp.y != 0) {
    return math::vector::normalized(-normalTmp);
  } else {
    return { 0, 0 };
  }
}

sf::Vector2f computeNormal(const Mask& mask, int accuracy, const sf::Vector2f& point) {
  return computeNormal(mask, accuracy, std::round(point.x), std::round(point.y));
}

}
}
}
