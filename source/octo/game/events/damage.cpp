#include "damage.hpp"

namespace octo {
namespace game {
namespace events {

Damage::Damage(entityx::Entity target, float damage)
  : target(target), damage(damage)
{
}

}
}
}
