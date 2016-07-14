#include "collisionmask.hpp"

namespace octo {
namespace game {
namespace components {

CollisionMask::CollisionMask() : mask(0, 0), anchor(0, 0) {}

CollisionMask::CollisionMask(collision::Mask mask) : mask(std::move(mask)), anchor(0, 0) {}

CollisionMask::CollisionMask(collision::Mask mask, sf::Vector2f anchor)
    : mask(std::move(mask)), anchor(anchor) {}
}
}
}
