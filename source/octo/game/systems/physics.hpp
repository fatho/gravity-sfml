#pragma once

#include "../components/spatial.hpp"
#include "../components/dynamicbody.hpp"
#include "../world.hpp"
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
  Physics(World& world);

  /*! \brief Performs physics calculations.
   *  \param es the entity system involved,
   *  \param events (currently) unused,
   *  \param dt used in the semi-implicit Euler integration. Using a fixed time-step is advisable.
   */
  void update(entityx::EntityManager& es, entityx::EventManager& events, entityx::TimeDelta dt) override;

private:
  /*! \brief Semi-implicit euler integration of movements.
   *  \brief es Entity manager.
   *  \brief timeStep The time in seconds since the last update.
   */
  void integrate(entityx::EntityManager& es, float timeStep);

private:
  fmtlog::Log log = fmtlog::For<Physics>();
  World& m_world;
};

}
}
}
