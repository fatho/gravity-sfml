#include "attractionsystem.hpp"

#include "../components/dynamicbody.hpp"
#include <octo/math/vector.hpp>

#include <cmath>
#include <iostream>

using namespace octo::game::systems;
using namespace entityx;

void Attraction::update(EntityManager& es, EventManager&, TimeDelta dt) {
  using namespace octo::game::components;
  // component iterators
  ComponentHandle<Spatial> attractedPos, attractorPos;
  ComponentHandle<Attractor> attractor;
  ComponentHandle<Attractable> attractable;
  ComponentHandle<DynamicBody> attractedBody;
  // traverse entities that can be attracted
  for(Entity e1 : es.entities_with_components(attractedPos, attractable, attractedBody)) {
    (void)e1;
    // traverse attracting entities
    for(Entity e2 : es.entities_with_components(attractorPos, attractor)) {
      (void)e2;
      if((attractable->attractionMask & attractor->attractionMask) != 0) {
        // compute attraction force towards attractor with quadratic falloff
        sf::Vector2f forceDir = attractorPos->current().position - attractedPos->current().position;
        float distanceSq = math::vector::lengthSquared(forceDir);
        float distance = static_cast<float>(sqrt(distanceSq));
        float radius = attractor->radius;
        float radiusSq = radius * radius;

        forceDir /= distance;
        sf::Vector2f force = forceDir * attractable->intensity * attractor->intensity;
        if(__builtin_expect(distanceSq < radiusSq, 0)) {
          // when inside the attractor, reduce force towards center based on an approximation
          // computed from the ratio of the two parts pulling the object further inwards,
          // and pulling it outwards.
          float alpha = distance * (3 * radiusSq -  distanceSq) / (2 * radiusSq * radius);
          force *= alpha / radiusSq;
          //std::cout << "force: " << math::vector::length(force) << "\n";
        } else {
          force /= distanceSq;
        }
        attractedBody->force += force;
      }
    }
  }
}
