#pragma once

#include <entityx/entityx.h>

namespace octo {
namespace game {
namespace events {

template <class T>
struct ComponentModified {
  entityx::Entity entity;
  entityx::ComponentHandle<T> component;

  ComponentModified(entityx::Entity entityArg, entityx::ComponentHandle<T> componentHandle)
      : entity(entityArg), component(componentHandle) {}
};

}
}
}
