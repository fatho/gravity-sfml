#pragma once

#include <entityx/entityx.h>
#include <SFML/System/Vector2.hpp>

namespace octo {
namespace game {
namespace events {

/*! \brief An event telling the explosion system to generate an explosion.
 */
struct Explode {
  sf::Vector2f center;
  float destructionRadius;
  float damageRadius;
  float force;
  float damage;

  Explode(const sf::Vector2f& center, float destructionRadius, float damageRadius, float force, float damage);
};

}
}
}
