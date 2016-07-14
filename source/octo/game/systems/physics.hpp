#pragma once

#include "../components/spatial.hpp"
#include "../components/dynamicbody.hpp"
#include <fmtlog/fmtlog.hpp> 

#include <entityx/entityx.h>

namespace octo {
namespace game {
namespace systems {

/*! \brief Responsible for updating the moving parts of the simulation and performs physics related computations.
 *
 *  In this system, forces are converted into motion using a semi-implicit Euler integration.
 */
struct Physics : public entityx::System<Physics> {
  /*! \brief Performs physics calculations.
   *  \param es the entity system involved,
   *  \param events (currently) unused,
   *  \param dt used in the semi-implicit Euler integration. Using a fixed time-step is advisable.
   */
  void update(entityx::EntityManager& es, entityx::EventManager& events, entityx::TimeDelta dt) override;

private:
  void integrate(entityx::EntityManager& es, float timeStep);

  void collisionDetection(entityx::EntityManager& es, float timeStep);

private:
  fmtlog::Log log = fmtlog::For<Physics>();
};

}
}
}
