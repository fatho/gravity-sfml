#include "explode.hpp"

namespace octo {
namespace game {
namespace events {

Explode::Explode(const sf::Vector2f& center, float destructionRadius, float damageRadius, float force, float damage)
  : center(center), destructionRadius(destructionRadius), damageRadius(damageRadius), force(force), damage(damage)
{
}

}
}
}
