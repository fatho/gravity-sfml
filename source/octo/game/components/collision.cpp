#include "collision.hpp"

namespace octo {
namespace game {
namespace components {

Collision::Collision() : mask(0, 0), anchor(0, 0) {}

Collision::Collision(collision::Mask mask) : mask(std::move(mask)), anchor(0, 0) {}

Collision::Collision(collision::Mask mask, sf::Vector2f anchor)
    : mask(std::move(mask)), anchor(anchor) {}
}
}
}
