#include "textureloader.hpp"

#include <SFML/Graphics/Texture.hpp>

using namespace octo::content::sfml;

std::shared_ptr<void> TextureLoader::load(sf::InputStream& stream) {
  auto tex = std::make_shared<sf::Texture>();
  if (!tex->loadFromStream(stream)) {
    throw ContentLoadException("failed to load texture");
  }
  return tex;
}
