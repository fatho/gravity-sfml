
#pragma once

#include <SFML/System/Vector2.hpp>

namespace octo {
namespace game {
namespace components {

struct Movable {
  sf::Vector2f velocity;
  sf::Vector2f acceleration;
};

}
}
}
