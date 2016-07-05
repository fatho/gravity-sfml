#pragma once

#include "spatial.hpp"

#include <Box2D/Dynamics/b2Body.h>
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
  std::unique_ptr<b2Body, internal::b2BodyDeleter> b2body;

  DynamicBody(b2Body* body) : b2body(body) {}

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
