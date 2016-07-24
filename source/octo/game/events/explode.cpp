#include "explode.hpp"

namespace octo {
namespace game {
namespace events {

Explode::Explode(const sf::Vector2f& center, float radius, float force, float damage)
  : center(center), radius(radius), force(force), damage(damage)
{
}

}
}
}
