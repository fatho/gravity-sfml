#include "vesselcontrol.hpp"

#include "../components.hpp"
#include <octo/math/all.hpp>

namespace octo {
namespace game {
namespace systems {

void VesselControl::configure(entityx::EventManager& events) {
  events.subscribe<events::Damage>(*this);
}

void VesselControl::update(entityx::EntityManager& es, entityx::EventManager& events,
                           entityx::TimeDelta dt) {
  using namespace octo::game::components;
  es.each<Vessel, Spatial, DynamicBody>(
      [&](entityx::Entity entity, Vessel& vessel, Spatial& pos, DynamicBody& body) {
        if (math::vector::lengthSquared(body.velocity()) <=
            math::util::sqr(m_levitationSpeedThreshold)) {
          // the vessel slow enough for the levitation engines
          sf::Transform vesselGlobal = pos.current().localToGlobal(true);
          const int numRays = 6;
          const float rayLen = 24;
          const float hwidth = 16;
          const float hheight = 12;
          for (int rayIdx = 0; rayIdx < numRays; ++rayIdx) {
            float angle = math::util::degToRad(rayIdx * (360.f / numRays));
            const float c = std::cos(angle);
            const float s = std::sin(angle);
            sf::Vector2f source =
                vesselGlobal.transformPoint(hwidth * std::cos(angle), hheight * std::sin(angle));
            sf::Vector2f target = vesselGlobal.transformPoint((rayLen - hwidth) * std::cos(angle),
                                                              (rayLen - hheight) * std::sin(angle));
            collision::bresenhamLine(source, target, [&](auto x, auto y) {a qq
                
              });
          }
        }
      });
}
}
}
}
