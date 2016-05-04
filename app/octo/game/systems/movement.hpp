#pragma once

#include "../components/position.hpp"
#include "../components/movable.hpp"

#include <entityx/entityx.h>

namespace octo {
namespace game {
namespace systems {

struct Movement : public entityx::System<Movement> {
  void update(entityx::EntityManager& es, entityx::EventManager&, entityx::TimeDelta dt) override;
};

}
}
}
