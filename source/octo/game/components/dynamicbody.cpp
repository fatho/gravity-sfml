#include "dynamicbody.hpp"

using namespace octo::game::components;

DynamicBody::DynamicBody()
  : mass(0), inverseMass(0)
{
}

DynamicBody::DynamicBody(float mass)
  : mass(mass), inverseMass(mass == 0 ? 0 : 1.0f / mass)
{
}
