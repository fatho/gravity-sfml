#include "dynamicbody.hpp"

#include <iostream>

#include <fmtlog/fmtlog.hpp>

#include <cmath>
#include <more-math/vector.hpp>

using namespace octo::game::components;

DynamicBody::DynamicBody(float mass, float inertia) {
  setMass(mass);
  setInertia(inertia);
}

void DynamicBody::setMass(float newmass) {
  float invm = 1.0f / newmass;
  if (std::isfinite(newmass) && std::isfinite(invm)) {
    this->mass = newmass;
    this->inverseMass = invm;
  } else {
    this->mass = this->inverseMass = 0;
  }
}

void DynamicBody::setInertia(float newinertia) {
  float invi = 1.0f / newinertia;
  if (std::isfinite(inertia) && std::isfinite(invi)) {
    this->inertia = newinertia;
    this->inverseInertia = invi;
  } else {
    this->inertia = this->inverseInertia = 0;
  }
}

void DynamicBody::applyForce(const sf::Vector2f& localPosition, const sf::Vector2f& appliedForce) {
  sf::Vector2f r = localPosition - centerOfMass;

  float generatedTorque = - math::vector::cross2d(r,  appliedForce);;

  fmtlog::Log(fmtlog::For<DynamicBody>())
      .debug("apply force {%f, %f} to {%f, %f}; r: {%f, %f} torque: %f",
             appliedForce.x,
             force.y,
             localPosition.x,
             localPosition.y,
             r.x,
             r.y,
             generatedTorque);

  // update force & torque
  this->force += appliedForce;
  this->torque += generatedTorque;
}

void DynamicBody::applyLinearImpulse(const sf::Vector2f& localPosition, const sf::Vector2f& impulse) {
  sf::Vector2f r = localPosition - centerOfMass;

  float angularImpulse = - math::vector::cross2d(r, impulse);

  fmtlog::Log(fmtlog::For<DynamicBody>())
    .debug("apply impulse {%f, %f} to {%f, %f}; r: {%f, %f} angular impulse: %f",
           impulse.x,
           impulse.y,
           localPosition.x,
           localPosition.y,
           r.x,
           r.y,
           angularImpulse);

  // update force & torque
  this->linearMomentum += impulse;
  this->angularMomentum += angularImpulse;
}
