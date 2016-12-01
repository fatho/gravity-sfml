#include "bounce.hpp"

#include "../components.hpp"
#include <octo/math/all.hpp>

#include <boost/math/constants/constants.hpp>

#include <algorithm>
#include <cmath>

using namespace octo::game::systems;

Bounce::Bounce() {}

void Bounce::update(entityx::EntityManager& es, entityx::EventManager&, entityx::TimeDelta dt) {
  std::for_each(begin(m_collisions), end(m_collisions), Bounce::bounce);
  m_collisions.clear();
}

void Bounce::configure(entityx::EventManager& events) {
  events.subscribe<events::EntityCollision>(*this);
}

void Bounce::receive(const events::EntityCollision& event) {
  m_collisions.push_back(event);
}

void Bounce::bounce(events::EntityCollision& colData) {
  std::array<entityx::ComponentHandle<components::Spatial>, 2> spatials;
  std::array<entityx::ComponentHandle<components::DynamicBody>, 2> bodies;
  std::array<entityx::ComponentHandle<components::Material>, 2> materials;
  std::array<sf::Vector2f, 2> lcont;
  std::array<sf::Vector2f, 2> contactMomenta;
  sf::Vector2f contactMomentum;
  int divisor = 0;
  for (int i = 0; i < 2; ++i) {
    spatials[i] = colData.entities[i].component<components::Spatial>();
    bodies[i] = colData.entities[i].component<components::DynamicBody>();
    materials[i] = colData.entities[i].component<components::Material>();
    if(!materials[i]) {
      // entities without material do not bounce
      return;
    }
    lcont[i] = spatials[i]->current().globalToLocal(false).transformPoint(colData.contactPoint);
    if (bodies[i].valid()) {
      contactMomenta[i] = bodies[i]->momentumAt(lcont[i]);
      contactMomentum += contactMomenta[i];
      divisor += 1;
    }
  }
  if (divisor == 0) {
    return;
  }
  for (int cur = 0; cur <= 1; ++cur) {
    int other = 1 - cur;
    if (spatials[cur].valid() && bodies[cur].valid()) {
      float dir = math::vector::dot(contactMomentum, colData.normals[other]);
      sf::Vector2f normalMomentum = colData.normals[other] * dir;

      if (dir < 0) {
        // only apply impulse when not moving outwards already
        sf::Vector2f tangentMomentum = contactMomentum - normalMomentum;
        const float restitution = std::max(materials[cur]->restitution, materials[other]->restitution);
        const float friction = std::sqrt(materials[cur]->friction * materials[other]->friction);
        bodies[cur]->applyLinearImpulse(lcont[cur],
                                        -normalMomentum * (1.f + restitution) /
                                                static_cast<float>(divisor) -
                                            tangentMomentum * friction,
                                        false);
      }
    }
  }
}
