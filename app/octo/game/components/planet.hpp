#pragma once

#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

namespace octo {
namespace game {
namespace components {

struct Planet {
  /// the image of the terrain used for physics
  sf::Image terrain;
  /// the replication of the terrain image in VRAM (slow to modify/query)
  sf::Texture terrainTexture;

  void updateTexture();
};

}
}
}
