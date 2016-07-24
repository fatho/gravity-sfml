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
  transform.translate(-0.5f * collision.size() + collision.anchor);
  transform.combine(spatial.localToGlobal());
  return transform;
}

inline sf::Transform globalToMask(const components::SpatialSnapshot& spatial,
                                  const components::CollisionMask& collision) {
  sf::Transform transform = spatial.globalToLocal();
  transform.translate(0.5f * collision.size() - collision.anchor);
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
}
}
}
