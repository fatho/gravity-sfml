#include "health.hpp"

#include <algorithm>

namespace octo {
namespace game {
namespace components {


void Health::heal(float amount) {
  currentHealth = std::min(currentHealth + amount, maximumHealth);
}

bool Health::damage(float amount) {
  currentHealth -= amount * damageResistance;
  if(currentHealth < 0) {
    currentHealth = 0;
    return true;
  } else {
    return false;
  }
}

}
}
}
