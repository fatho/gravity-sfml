#pragma once

#include <entityx/entityx.h>
#include <SFML/System/Vector2.hpp>

namespace octo {
namespace game {
namespace events {

/*! \brief An event telling the explosion system to generate an explosion.
 */
struct Explode {
  /*! \brief The entity causing the explosion.
   *  \remark This entity will remain unaffected by explosion.
   */
  entityx::Entity origin;
  /// The center where the explosion is originating from.
  sf::Vector2f center;
  /// Destructible parts of collision masks within this radius from the center are destroyed.
  float destructionRadius;
  /// Entities within this radius of the center receive damage and are pushed outwards.
  float damageRadius;
  /*! \brief The maximum force that would be applied in the center of the explosion.
   *  \remark Since there is no sensible way of calculating the direction of the force
   *  when being so close to the center, the push is only applied when the affected entity
   *  is a small distance away.
   */
  float force;
  /// The maximum damage that is applied to entities right at the center of the explosion.
  float damage;

  Explode(entityx::Entity origin, const sf::Vector2f& center, float destructionRadius, float damageRadius, float force, float damage);
};

}
}
}
