#include "entitycollision.hpp"

namespace octo {
namespace game {
namespace events {

EntityCollision::EntityCollision(const std::array<entityx::Entity, 2>& collisionEntities,
                const std::array<sf::Vector2f, 2>& collisionNormals,
                const sf::Vector2f& contactPoint)
  : entities(collisionEntities), normals(collisionNormals), contactPoint(contactPoint)
{
}

void EntityCollision::swapOrder() {
  std::reverse(begin(entities), end(entities));
  std::reverse(begin(normals), end(normals));
}

}
}
}
