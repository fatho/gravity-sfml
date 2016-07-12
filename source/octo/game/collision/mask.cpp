#include "mask.hpp"

namespace octo {
namespace game {
namespace collision {

Mask circle(size_t radius, Pixel fill) {
  Mask mask(radius * 2, radius * 2, Pixel::NoCollision);
  int radiusSq = radius * radius;
  for(int y : mask.yrange()) {
    for(int x : mask.xrange()) {
      int rx = x - radius;
      int ry = y - radius;
      if(rx * rx + ry * ry <= radiusSq) {
        mask.at(x, y) = fill;
      }
    }
  }
  return mask;
}

Mask rectangle(size_t width, size_t height, Pixel fill) {
  return Mask(width, height, fill);
}

}
}
}
