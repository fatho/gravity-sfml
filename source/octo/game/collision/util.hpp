#pragma once

#include "../components.hpp"
#include "mask.hpp"
#include <octo/math/rect.hpp>

#include <SFML/Graphics/Rect.hpp>
#include <entityx/entityx.h>

namespace octo {
namespace game {
namespace collision {

inline sf::Transform maskToGlobal(const components::SpatialSnapshot& spatial,
                                  const components::CollisionMask& collision) {
  sf::Transform transform;
  transform.combine(spatial.localToGlobal());
  transform.translate(-0.5f * collision.size() + collision.anchor);
  return transform;
}

inline sf::Transform globalToMask(const components::SpatialSnapshot& spatial,
                                  const components::CollisionMask& collision) {
  sf::Transform transform;
  transform.translate(0.5f * collision.size() - collision.anchor);
  transform.combine(spatial.globalToLocal());
  return transform;
}

struct AabbQueryData {
  entityx::Entity entity;
  components::Spatial& spatialComponent;
  components::CollisionMask& collisionComponent;
  sf::Transform maskToGlobal{};
  sf::Transform globalToMask{};
  sf::FloatRect intersectionGlobal{};
  sf::Rect<size_t> intersectionMask{};
};

template <typename F>
void aabbQuery(entityx::EntityManager& em, const sf::FloatRect& queryAABB, F callback) {
  using namespace components;
  em.each<Spatial, CollisionMask>(
      [&](entityx::Entity obj, Spatial& spatial, CollisionMask& collision) {
        AabbQueryData data{obj, spatial, collision};
        data.maskToGlobal = maskToGlobal(spatial.current(), collision);
        sf::FloatRect localBounds = {{0.f, 0.f}, collision.size()};
        sf::FloatRect objAABB = data.maskToGlobal.transformRect(localBounds);
        if (objAABB.intersects(queryAABB, data.intersectionGlobal)) {
          data.globalToMask = globalToMask(spatial.current(), collision);
          sf::FloatRect localIntersection;
          localBounds.intersects(data.globalToMask.transformRect(data.intersectionGlobal),
                                 localIntersection);
          data.intersectionMask = math::rect::integralOutwards<size_t>(localIntersection);
          callback(obj, data);
        }
      });
}

/*! \brief Approximates the surface normal by averaging over all solid pixels
 *  in a given square around the point of interest.
 *
 *  All vectors from \c (x,y) to solid pixels in a \c accuracy*2+1 square around
 *  that point are added, normalized and inverted.
 *
 *  \param mask the collision mask.
 *
 *  \param accuracy the accuracy of the computation. The runtime is roughly proportional
 *  to the square of \p accuracy.
 *
 *  \param x the x coordinate of the point of interest
 *  \param y the y coordinate of the point of interest
 *
 *  \returns an approximated surface normal at \c (x,y).
 */
sf::Vector2f computeNormal(const Mask& mask, int accuracy, size_t x, size_t y);

sf::Vector2f computeNormal(const Mask& mask, int accuracy, const sf::Vector2f& point);

}
}
}
