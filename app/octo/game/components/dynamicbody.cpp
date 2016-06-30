#include "dynamicbody.hpp"

using namespace octo::game::components;

DynamicBody::DynamicBody()
  : mass(0), inverseMass(0), affectedByGravity(false)
{
}

DynamicBody::DynamicBody(float mass, bool affectedByGravity)
  : mass(mass), inverseMass(1.0f / mass), affectedByGravity(affectedByGravity)
{
}
