#pragma once

#include <SFML/System/Vector2.hpp>
#include <cmath>

namespace octo {
  namespace math {
    namespace vector {

      template<typename T>
      inline T dot(const sf::Vector2<T>& va, const sf::Vector2<T>& vb) {
        return va.x * vb.x + va.y * vb.y;
      }

      template<typename T>
      inline T lengthSquared(const sf::Vector2<T>& vec) {
        return dot(vec, vec);
      }

      template<typename T>
      inline T length(const sf::Vector2<T>& vec) {
        return static_cast<T>(sqrt(lengthSquared(vec)));
      }
    }
  }
}
