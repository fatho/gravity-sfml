#pragma once

#include <entityx/entityx.h>

namespace octo {
namespace game {
namespace events {

/*! \brief An event telling the HealthSystem to damage an entity.
 */
struct Damage {
  entityx::Entity target;
  float damage;

  Damage(entityx::Entity target, float damage);
};

}
}
}
