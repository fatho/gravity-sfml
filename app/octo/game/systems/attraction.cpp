#include "attraction.hpp"

#include "../components/dynamicbody.hpp"
#include <math/vector.hpp>

#include <cmath>
#include <iostream>

using namespace octo::game::systems;
using namespace entityx;

void Attraction::update(EntityManager& es, EventManager&, TimeDelta dt) {
  using namespace octo::game::components;
  // component iterators
  ComponentHandle<Position> attractedPos, attractorPos;
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
        sf::Vector2f forceDir = attractorPos->position - attractedPos->position;
        float distanceSq = math::vector::lengthSquared(forceDir);
        forceDir /= static_cast<float>(sqrt(distanceSq));
        sf::Vector2f force = forceDir * attractable->forceFactor * attractor->intensity / distanceSq;
        //std::cout << "attraction: " << force.x << " " << force.y << "\n";
        attractedBody->force += force;
      }
    }
  }
}
