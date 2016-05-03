#include "sfml.hpp"
#include "contentmanager.hpp"
#include "sfml/textureloader.hpp"

#include <SFML/Graphics.hpp>

void octo::content::sfml::registerSFMLLoaders(octo::content::ContentManager& manager) {
  manager.registerLoader<sf::Texture>(std::make_unique<TextureLoader>());
  // TODO: add other SFML content handlers
}
