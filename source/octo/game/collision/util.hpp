#pragma once

#include "../components.hpp"
#include "mask.hpp"
#include <octo/math/rect.hpp>

#include <SFML/Graphics/Rect.hpp>
#include <entityx/entityx.h>

#include <algorithm>

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

template <typename Callback>
sf::Vector2f bresenhamLine(sf::Vector2f& start, sf::Vector2f& end, Callback callback) {
  const bool steep = (std::fabs(end.y - start.y) > std::fabs(end.x - start.x));
  if (steep) {
    std::swap(start.x, start.y);
    std::swap(end.x, end.y);
  }

  if (start.x > end.x) {
    std::swap(start.x, end.x);
    std::swap(start.y, end.y);
  }

  const float dx = end.x - start.x;
  const float dy = fabs(end.y - start.y);

  float error = dx / 2.0f;
  const int ystep = (start.y < end.y) ? 1 : -1;
  int y = static_cast<int>(start.y);

  const int maxX = static_cast<int>(end.x);

  sf::Vector2f cur;

  for (int x = static_cast<int>(start.x); x < maxX; x++) {
    bool ret;
    if (steep) {
      cur = { static_cast<float>(y), static_cast<float>(x) };
      ret = callback(y, x);
    } else {
      cur = { static_cast<float>(x), static_cast<float>(y) };
      ret = callback(x, y);
    }
    if(! ret) {
      return cur;
    }

    error -= dy;
    if (error < 0) {
      y += ystep;
      error += dx;
    }
  }
  return cur;
}
}
}
}
