#include "mask.hpp"

namespace octo {
namespace game {
namespace collision {

Mask circle(size_t radius, Pixel fill) {
  Mask mask(radius * 2 + 1, radius * 2 + 1, Pixel::NoCollision);
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

Mask ellipse(size_t width, size_t height, Pixel fill) {
  Mask mask(width, height, Pixel::NoCollision);
  const float hx = static_cast<float>(width - 1) / 2.f;
  const float hy = static_cast<float>(height - 1) / 2.f;
  const float hx2 = hx * hx;
  const float hy2 = hy * hy;
  if(hx2 > 0 && hy2 > 0) {
    for(int y : mask.yrange()) {
      for(int x : mask.xrange()) {
        float rx = x - hx;
        float ry = y - hy;
        if(rx * rx / hx2 + ry * ry / hy2 <= 1.f) {
          mask.at(x, y) = fill;
        }
      }
    }
  }
  return mask;
}

}
}
}
