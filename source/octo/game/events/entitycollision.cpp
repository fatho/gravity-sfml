#include "entitycollision.hpp"

namespace octo {
namespace game {
namespace events {


void EntityCollision::swapOrder() {
  std::swap(entity[0], entity[1]);
}

}
}
}
