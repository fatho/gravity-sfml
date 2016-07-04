#include "dynamicbody.hpp"

#include <more-math/vector.hpp>
#include <cmath>

using namespace octo::game::components;

DynamicBody::DynamicBody()
  : mass(0), inverseMass(0)
{
}

DynamicBody::DynamicBody(float mass, float inertia)
{
  setMass(mass);
  setInertia(inertia);
}

void DynamicBody::setMass(float newmass) {
  float invm = 1.0f / newmass;
  if(std::isfinite(newmass) && std::isfinite(invm)) {
    this->mass = newmass;
    this->inverseMass = invm;
  } else {
    this->mass = this->inverseMass = 0;
  }
}

void DynamicBody::setInertia(float newinertia) {
  float invi = 1.0f / newinertia;
  if(std::isfinite(inertia) && std::isfinite(invi)) {
    this->inertia = newinertia;
    this->inverseInertia = invi;
  } else {
    this->inertia = this->inverseInertia = 0;
  }
}

void DynamicBody::applyForce(const sf::Vector2f& localPosition, const sf::Vector2f& force) {
  sf::Vector2f r = localPosition - centerOfMass;
  sf::Vector2f rNorm = math::vector::normalized(r);

  // compute linear part of force
  sf::Vector2f Flin = force * math::vector::dot(rNorm, force);

  // compute (remaining) rotational part of force
  sf::Vector2f Frot = force - Flin;

  // update force & torque
  this->force += Flin;
  this->torque += r.x * Frot.y - r.y * Frot.x;
}
