#pragma once

namespace octo {
namespace game {
namespace components {

/*! \brief Defines the material of an entity.
 *
 *  The material controls how an entity reacts to collisions. Otherwise,
 *  collisions are only detected, but not handled.
 */
struct Material {
  float restitution;
  float friction;

  Material(float restitution, float friction);
};

}
}
}
