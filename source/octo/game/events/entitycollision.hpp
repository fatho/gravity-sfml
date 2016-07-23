#pragma once

#include <entityx/entityx.h>
#include <SFML/System/Vector2.hpp>

namespace octo {
namespace game {
namespace events {

struct EntityCollision {
  entityx::Entity entity[2];
  sf::Vector2f contactPoint;
};

}
}
}
