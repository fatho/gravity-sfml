#pragma once

#include "../components/position.hpp"
#include "../components/dynamicbody.hpp"

#include <entityx/entityx.h>

namespace octo {
namespace game {
namespace systems {

struct Physics : public entityx::System<Physics> {
  void update(entityx::EntityManager& es, entityx::EventManager&, entityx::TimeDelta dt) override;
};

}
}
}
