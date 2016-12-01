#include "explode.hpp"

namespace octo {
namespace game {
namespace events {

Explode::Explode(entityx::Entity origin, const sf::Vector2f& center, float destructionRadius, float damageRadius, float force, float damage)
  : origin(origin), center(center), destructionRadius(destructionRadius), damageRadius(damageRadius), force(force), damage(damage)
{
}

}
}
}
