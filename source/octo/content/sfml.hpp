#pragma once

#include "contentmanager.hpp"

namespace octo {
namespace content {

class ContentManager;

namespace sfml {

/*! \brief A generic content loader that works for most simple SFML content objects.
 *  The know supported SFML classes are sf::Texture, sf::Image, sf::Font and sf::SoundBuffer.
 *  \note This class neither supports sf::Music, which is instead handled by MusicLoader,
 *  nor sf::Shader, handled by ShaderLoader.
 */
template<typename ContentType>
class BasicSFMLLoader : public ContentLoader {
  // ContentLoader interface
public:
  std::shared_ptr<void> load(ContentManager& manager, const std::string& contentPath) override {
    auto stream = manager.openDataStream(contentPath);
    auto content = std::make_shared<ContentType>();
    if (stream && content->loadFromStream(*stream)) {
      return content;
    } else {
      throw ContentLoadException("failed to load content");
    }
  }
};

/*! \brief Helper function for registering the default loader for a given basic SFML content type.
 */
template<typename ContentType>
static void registerBasicSFMLLoader(octo::content::ContentManager& manager) {
  manager.registerLoader<ContentType>(std::make_unique<BasicSFMLLoader<ContentType>>());
}

/*! \brief registers default loaders for all known supported SFML content types.
 *  This includes MusicLoader and ShaderLoader.
 */
void registerSFMLLoaders(content::ContentManager& manager);

} // namespace sfml
} // namespace content
} // namespace octo
