
#pragma once

#include <SFML/System/Vector2.hpp>

namespace octo {
namespace game {
namespace components {

struct DynamicBody {
  float mass;
  float inverseMass;
  sf::Vector2f momentum;
  sf::Vector2f force;
  bool affectedByGravity;

  DynamicBody();

  DynamicBody(float mass, bool affectedByGravity);

  sf::Vector2f velocity() const {
    return momentum * inverseMass;
  }

  void setVelocity(sf::Vector2f velocity) {
    this->momentum = velocity * this->mass;
  }

  void setMass(float mass) {
    if(mass != 0) {
      this->mass = mass;
      this->inverseMass = 1.0f / mass;
    } else {
      this->mass = this->inverseMass = 0;
    }
  }
};

}
}
}
