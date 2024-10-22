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
public:
  //////////////////////////////////////////////////////////////////////////////
  // Data Members

  /// the mass of the body
  float mass = 0;
  /// the inverse of the bodies mass, mainly useful when having bodies with infinite mass.
  float inverseMass = 0;
  /*! \brief The current linear momentum \f$ \mathbf{p} \f$ of the body.
   *
   *  Classical mechanics tell us that \f$ \mathbf{p} = m \mathbf{v} \f$.
   *  The first time derivative of momentum is force.
   */
  sf::Vector2f linearMomentum = sf::Vector2f();
  /*! \brief The current force acting on the body's center of mass.
   *
   *  The physics engine is integrating the force, resulting in linear momentum.
   */
  sf::Vector2f force = sf::Vector2f();

  /*! \brief The current angular momentum \f$ L \f$ of the body.
   *
   *  It is related to angular velocity by \f$ L = I \omega \f$, where \f$ I \f$ denotes
   *  the bodies inertia. Since we are in a two dimensional space, there is only one rotation
   *  axis, the one through the body's center of mass.
   *  The torque is the first time derivative of angular momentum.
   */
  float angularMomentum = 0;

  /*! \brief The current torque acting on the body.
   *
   *  The physics engine is integrating the torque, resulting in angular momentum.
   */
  float torque = 0;

  /*! \brief The bodies center of mass in its local coordinate system.
   *  It is relevant in the cases where force is applied outside of the center of mass.
   */
  sf::Vector2f centerOfMass = sf::Vector2f();

  /*! \brief The bodies inertia w.r.t. the only rotation axis in 2D space.
   */
  float inertia = 0;

  /*! \brief The bodies inverse inertia.
   *
   *  Mainly useful when having a body with an infinite mass or with a fixed rotation.
   */
  float inverseInertia = 0;

  /*! \brief When a body is sleeping, it will not be affected forces and won't move.
   *
   *  \remark A sleeping body retains its momentum.
   */
  bool sleeping = false;

public:
  //////////////////////////////////////////////////////////////////////////////
  // Methods & Constructors


  /*! \brief Initializes the body with a given mass and inertia.
   *
   *  It defaults to a body with an infinite mass.
   *
   *  \param mass the initial mass
   *  \param inertia the initial inertia.
   */
  DynamicBody(float mass = 0, float inertia = 0);

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
   *  Forces applied outside of the center of mass usually generate torque as well.
   *
   *  \param localPosition the point of the body in local coordinates the force is applied to.
   *  \param force the force that is applied.
   */
  void applyForce(const sf::Vector2f& localPosition, const sf::Vector2f& force, bool wakeUp = true);

  /*! \brief Applies a linear impulse at a specific point of the body.
   *
   *  Linear impulses applied outside of the center of mass usually results
   *  in a change of angular momentum as well.
   *
   *  \param localPosition the point of the body in local coordinates the impulse is applied to.
   *  \param impulse the linear impulse that is applied.
   */
  void applyLinearImpulse(const sf::Vector2f& localPosition, const sf::Vector2f& impulse, bool wakeUp = true);

  sf::Vector2f momentumAt(const sf::Vector2f& localPosition);
};

}
}
}
