
#pragma once

#include <SFML/System/Vector2.hpp>

namespace octo {
namespace game {
namespace components {

/*! \brief Dynamic bodies are those entities that are affected by the physics engine.
 *
 *  Their behavior is determined by their mass, momentum and the force acting on them.
 */
struct DynamicBody {
  /// the mass of the body
  float mass;
  /// the inverse of the bodies mass, mainly useful when having bodies with infinite mass.
  float inverseMass;
  /*! \brief The current momentum of the body.
   *
   *  Classical mechanics tell us that \f$ \mathbf{p} = m \mathbf{v} \f$.
   *  The first time derivative of momentum is force.
   */
  sf::Vector2f momentum;
  /*! \brief The current force acting on the body.
   *
   *  The physics engine is integrating the force, resulting in momentum.
   */
  sf::Vector2f force;

  /// Resting body with infinite mass.
  DynamicBody();

  /*! \brief Initializes the body with a given mass.
   *  \param mass the initial mass of the body.
   */
  DynamicBody(float mass);

  /*! \brief Computes the current velocity of the body.
   *
   *  Velocity \f$ \mathbf{v} = \frac{\mathbf{p}}{m} \f$.
   */
  sf::Vector2f velocity() const {
    return momentum * inverseMass;
  }

  /*! \brief Sets the velocity of the body.
   *
   *  Internally, the momentum is updated according to \f$ \mathbf{p} = m \mathbf{v} \f$.
   */
  void setVelocity(sf::Vector2f velocity) {
    this->momentum = velocity * this->mass;
  }

  /*! \brief Sets the mass of the body.
   *
   *  This function also updates the inverse mass.
   *  \param mass the new mass.
   */
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
