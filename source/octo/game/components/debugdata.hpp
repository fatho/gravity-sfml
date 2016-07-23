#pragma once

#include <SFML/Graphics/Texture.hpp>

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
