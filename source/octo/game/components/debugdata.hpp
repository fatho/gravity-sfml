#pragma once

#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

namespace octo {
namespace game {
namespace components {

/*! \brief Component storing data only relevant for debugging.
 */
struct DebugData {
  sf::Texture collisionMaskTexture;
};

}
}
}
