#pragma once

#include "spatial.hpp"

#include <Box2D/Dynamics/b2Body.h>
#include <entityx/Entity.h>
#include <SFML/System/Vector2.hpp>

#include <memory>

namespace octo {
namespace game {
namespace components {

namespace internal {

struct b2BodyDeleter {
  void operator()(b2Body* body);
};
}

/*! \brief Dynamic bodies are those entities that are affected by the physics engine.
 *
 */
struct DynamicBody {
  /// the Box2D managed body.
  std::unique_ptr<b2Body, internal::b2BodyDeleter> b2body;

  /// a reference to the owning entity, used for b2Body user data
  entityx::Entity entity;

  DynamicBody(b2Body* body) : b2body(body) {}

  // make no sense to copy or move this. it just lives as long as the associated entity
  DynamicBody(const DynamicBody&) = delete;
  DynamicBody& operator=(const DynamicBody&) = delete;
  DynamicBody(DynamicBody&&) = delete;
  DynamicBody& operator=(DynamicBody&&) = delete;

  b2Body* operator->() {
    return b2body.get();
  }

  const b2Body* operator->() const {
    return b2body.get();
  }
};
}
}
}
