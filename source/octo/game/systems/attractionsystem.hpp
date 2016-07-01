#pragma once

#include "../components/position.hpp"
#include "../components/attraction.hpp"

#include <entityx/entityx.h>

namespace octo {
namespace game {
namespace systems {

struct Attraction : public entityx::System<Attraction> {
  void update(entityx::EntityManager& es, entityx::EventManager&, entityx::TimeDelta dt) override;
};

}
}
}
