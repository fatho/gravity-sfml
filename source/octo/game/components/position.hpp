#pragma once

#include <SFML/System/Vector2.hpp>

namespace octo {
namespace game {
namespace components {

struct Position {
  Position(sf::Vector2f pos);

  sf::Vector2f position;
};

}
}
}
