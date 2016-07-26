#pragma once

namespace octo {
namespace game {
namespace components {

/*! \brief Defines properties for all entities with some kind of 'health' value.
 */
struct Health {
  /// the maximum health the entity can have
  float maximumHealth;
  /// the current health of the entity
  float currentHealth;
  /// the percentage of incoming damage that is absorbed
  float damageResistance;

  /*! \brief Heals the entity by the specific amount, without exceeding the \ref maximumHealth.
   *  \param amount the amount of health restored.
   */
  void heal(float amount);

  /*! \brief Damages the entity.
   *
   *  Incoming damage is subject to \ref damageResistance.
   *  \param amount the amount of damage applied to the entity.
   *  \returns \c true if the entity was killed, i.e. if the health dropped
   *  to zero, \c false if it is still alive.
   */
  bool damage(float amount);
};

}
}
}
