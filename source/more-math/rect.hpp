#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>

namespace octo {
namespace math {
namespace rect {

template<typename T>
sf::Rect<T> fromCenterSize(const sf::Vector2<T>& center, const sf::Vector2<T>& size) {
  return sf::Rect<T>(center - size / static_cast<T>(2), size);
}

}
}
}
