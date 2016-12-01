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
  std::array<sf::Vector2f, 2> lcont;
  std::array<sf::Vector2f, 2> contactMomenta;
  sf::Vector2f contactMomentum;
  int divisor = 0;
  // compute per-entity data
  for (int i = 0; i < 2; ++i) {
    if(colData.entities[i].component<components::CollisionMask>()->sensor) {
      // if a sensor is involved, don't process.
      return;
    }
    spatials[i] = colData.entities[i].component<components::Spatial>();
    bodies[i] = colData.entities[i].component<components::DynamicBody>();
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
  // apply reaction to all involved entities
  for (int cur = 0; cur <= 1; ++cur) {
    int other = 1 - cur;
    if (spatials[cur].valid() && bodies[cur].valid()) {
      float dir = math::vector::dot(contactMomentum, colData.normals[other]);
      sf::Vector2f normalMomentum = colData.normals[other] * dir;

      if (dir < 0) {
        // only apply impulse when not moving outwards already
        sf::Vector2f tangentMomentum = contactMomentum - normalMomentum;
        float restitution = 0.8f;
        float friction = 0.1f;
        bodies[cur]->applyLinearImpulse(lcont[cur],
                                        -normalMomentum * (1.f + restitution) /
                                                static_cast<float>(divisor) -
                                            tangentMomentum * friction,
                                        false);
      }
    }
  }
}
