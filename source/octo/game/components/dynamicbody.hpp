
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
  /*! \brief The current linear momentum \f$ \mathbf{p} \f$ of the body.
   *
   *  Classical mechanics tell us that \f$ \mathbf{p} = m \mathbf{v} \f$.
   *  The first time derivative of momentum is force.
   */
  sf::Vector2f linearMomentum;
  /*! \brief The current force acting on the body's center of mass.
   *
   *  The physics engine is integrating the force, resulting in linear momentum.
   */
  sf::Vector2f force;

  /*! \brief The current angular momentum \f$ L \f$ of the body.
   *
   *  It is related to angular velocity by \f$ L = I \omega \f$, where \f$ I \f$ denotes
   *  the bodies inertia. Since we are in a two dimensional space, there is only one rotation
   *  axis, the one through the body's center of mass.
   *  The torque is the first time derivative of angular momentum.
   */
  float angularMomentum;

  /*! \brief The current torque acting on the body.
   *
   *  The physics engine is integrating the torque, resulting in angular momentum.
   */
  float torque;

  /*! \brief The bodies center of mass in its local coordinate system.
   *  It is relevant in the cases where force is applied outside of the center of mass.
   */
  sf::Vector2f centerOfMass;

  /*! \brief The bodies inertia w.r.t. the only rotation axis in 2D space.
   */
  float inertia;
  /*! \brief The bodies inverse inertia.
   *
   *  Mainly useful when having a body with an infinite mass or with a fixed rotation.
   */
  float inverseInertia;

  /// Resting body with infinite mass.
  DynamicBody();

  /*! \brief Initializes the body with a given mass and inertia.
   *  \param mass the initial mass of the body.
   *  \param inertia the bodies inertia.
   */
  DynamicBody(float mass, float inertia);

  /*! \brief Computes the current velocity of the body.
   *
   *  Velocity \f$ \mathbf{v} = \frac{\mathbf{p}}{m} \f$.
   */
  sf::Vector2f velocity() const {
    return linearMomentum * inverseMass;
  }

  /*! \brief Sets the velocity of the body.
   *
   *  Internally, the momentum is updated according to \f$ \mathbf{p} = m \mathbf{v} \f$.
   */
  void setVelocity(sf::Vector2f velocity) {
    linearMomentum = velocity * mass;
  }

  /*! \brief Computes the current angular velocity of the body.
   *
   *  Angular velocity \f$ \omega = \frac{L}{I} \f$.
   *  \return the current angular velocity in radians per second.
   */
  float angularVelocity() const {
    return angularMomentum * inverseInertia;
  }

  /*! \brief Sets the mass of the body.
   *
   *  This function also updates the inverse mass.
   *  \param mass the new mass.
   */
  void setMass(float mass);

  /*! \brief Sets the inertia of the body.
   *
   *  This function also updates the inverse inertia.
   *  \param inertia the new inertia.
   */
  void setInertia(float inertia);

  /*! \brief Applies a force at a specific point of the body.
   *
   *  The applied force is split into the part that is acting on the center of mass,
   *  and into the torque.
   *
   *  \param localPosition the point of the body in local coordinates the force is applied to.
   *  \param force the force that is applied.
   */
  void applyForce(const sf::Vector2f& localPosition, const sf::Vector2f& force);
};

}
}
}
