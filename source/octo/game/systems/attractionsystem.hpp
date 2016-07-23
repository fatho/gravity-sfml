#pragma once

#include <entityx/entityx.h>

namespace octo {
namespace game {
namespace systems {

/*! \brief This system applies the attractive forces between entities defined by
 *  the \ref components::Attractor and \ref components::Attractable components.
 *
 *  Near infinite forces that would occur when the distance between the entities
 *  get to close to each other are worked around by defining a separate approximation
 *  for being inside of some radius of the attractor. In case of a planet, this would be
 *  the planets radius.
 *
 *  \ŗemark
 *  Given
 *    - \f$ r \f$, the radius of the attractor,
 *    - \f$ i_1, i_2 \f$, the intesity of the attractor resp. attractable, and
 *    - \f$ d \f$, the distance between the positions of the two entities.
 *
 *  The calculation assumes that the forces are always exerted from/towards the entities centers.
 *
 *  When \f$ d >= r \f$, i.e. when the attractable is (just) outside of the attractor,
 *  the magnitude of the force is computed as \f[ |F| = \frac{i_1 i_2}{d^2}. \f]
 *
 *  When \f$ d < r \f$, i.e. when the attractable is inside the attractor,
 *  the magnitude of the force is computed as \f[ |F| = \alpha \frac{i_1 i_2}{r^2}, \f]
 *  where \f$ \alpha \f$ is an approximation of the ratio of the part of the attractor that
 *  is still pulling towards the center, and the part that is now pulling outside.
 *  The weights of the parts are computed from an imaginary sphere around the attractors center
 *  with radius \f$ r \f$. This leads to \f[ \alpha = \frac{d(3r^2 - d^2)}{2r^3}. \f]
 *
 *  Notably, when \f$ d = r \f$ (the attractable is just touching the outer limit), we have \f$ \alpha = 1 \f$,
 *  and for \f$ d = 0 \f$, we have \f$ \alpha = 0 \f$.
 *  Hence, there is no sudden change in force at the boundary. However, this approximation does not
 *  preserve the derivative of the force (somehow related to the physical measure of "jerk") at the boundary.
 *
 */
struct Attraction : public entityx::System<Attraction> {
  /*! \brief Calculates and adds the attractive forces.
   *
   *  Only entities having a \ref components::Spatial component are considered.
   *  Furthermore, \ref components::Attractable "Attractables" must also be \ref components::DynamicBody "DynamicBodies"
   *  in order to be affected by forces.
   *
   *  \param es the entity system involved,
   *  \param events unused,
   *  \param dt unused, the forces themselves do not depend on time.
   *
   *  \idea provide different kinds of falloffs
   */
  void update(entityx::EntityManager& es, entityx::EventManager& events, entityx::TimeDelta dt) override;
};

}
}
}
