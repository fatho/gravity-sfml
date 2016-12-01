#pragma once

#include "../components.hpp"
#include "../events/damage.hpp"

#include <fmtlog/fmtlog.hpp>
#include <entityx/entityx.h>

namespace octo {
namespace game {
namespace systems {

/*!
 */
struct VesselControl : public entityx::System<VesselControl>, public entityx::Receiver<VesselControl> {

  void configure(entityx::EventManager& events) override;

  void update(entityx::EntityManager& es, entityx::EventManager& events,
              entityx::TimeDelta dt) override;
private:
  fmtlog::Log log = fmtlog::For<VesselControl>();
  float m_levitationSpeedThreshold = 5;
};
}
}
}
